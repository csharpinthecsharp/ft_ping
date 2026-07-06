#include "../include/header.h"

bool stop = false;

static void
sigintHandler(int sig)
{
	stop = true;
}

int
main(int ac, char **av)
{
	if (getuid() != 0)
		ft_error("ft_ping", "Require root permission");
	if (ac < 2)		
		err_help("missing host operand", NULL);

	signal(SIGINT, sigintHandler);

	check_help(ac, av);
	t_net net;
	init_struct(&net);
	net.len_addrs = get_addrs_size(ac, av);
	net.ad = malloc(sizeof(t_addr) * (net.len_addrs + 1));
	if (!net.ad)
		ft_error("malloc", "failed to allocate addrs f[main.c]");
	parse_args(&net, ac, av);
	net.sockfd = create_socket();
	get_ping_infos(&net);
	
	uint16_t seq = htons(0);
	while (!stop) {
		gettimeofday(&net.t_start, NULL);
		uint8_t buf[64];
		gen_header(buf, sizeof(buf), seq);
		send_packet(net.sockfd, buf, sizeof(buf), (struct sockaddr*)net.ad[0].ip, sizeof(struct sockaddr_in));
		if (recv_packet(&net, ntohs(seq), net.sockfd, sizeof(buf), buf) != -1) {
			seq += htons(1);
			usleep(1000000);
		}	
	}
	
	int int_seq = ntohs(seq);
	float avg = net.ms_total / int_seq;
	float stddev = sqrt((net.ms_total2 / int_seq) - (avg * avg));
	fprintf(stdout, "--- %s ping statistics ---\n"
			"%ld packets transmitted, %d packets received, %ld%% packet loss\n"
			"round-trip min/avg/max/stddev = %1.3f/%1.3f/%1.3f/%1.3f ms\n",
		       		net.ad[0].addr, int_seq, net.p_succ, ((net.p_lost * 100) / int_seq),
					net.ms_min, avg,
				       	net.ms_max, stddev);

	if (net.wrong_host_mltp) {
		free_struct(&net);
		ft_error("ft_ping", "unknown host");
	}

	free_struct(&net);
}
