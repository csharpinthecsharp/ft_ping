#include "header.h"

bool stop = false;

static void
sigintHandler(int sig)
{
	stop = true;
}

static void
findAddr(char **dest, const char* src) {
	if (!strcmp("-v", src))
		return;
	(*dest) = strdup(src);
}

static size_t
get_addrs_size(size_t len, char **srcs) {
	size_t i = 1;
	size_t count = 0;
	while (i < len) {
		if (!strcmp("-v", srcs[i])) {
			i++;
			continue;
		}
		i++;
		count++;
	}
	return (count);
}

static void
parse_args (t_net *net, int ac, char **av) {	
	size_t i = 1;
	size_t j = 0;
	for (int i = 0; i < net->len_addrs; i++) {
		net->ad[i].addr = NULL;
		net->ad[i].len_addr = 0;
		net->ad[i].print_ip = NULL;
		net->ad[i].ip = NULL;
	}

	while (i < ac) {
		if (!strcmp("-v", av[i])) {
			net->f_verbose = true;
	       		i++;
		}
		if (j < net->len_addrs) {
			findAddr(&net->ad[j].addr, av[i]);
			j++;
		}
		i++;
	}
	if (net->len_addrs == 0)
		ft_error("usage error", "Require destination address");
}

static void
isHelp(size_t len, char **srcs) {
	for (size_t i = 1; i < len; i++) {
		if (!strcmp(srcs[i], "-?"))
			ft_help();
	}
}

static void
init_struct(t_net *net) {
	net->ad = NULL;
	net->f_verbose = false;
	net->len_addrs = 0;
	net->sockfd = -1;
	net->p_lost = 0;
	net->p_succ = 0;
}

uint16_t
get_checksum(void *buf, size_t len)
{
	uint8_t *dt = (uint8_t*)buf;
	uint32_t sum = 0;

	for (size_t i = 0; i < len; i+=2) {
		uint16_t w = (dt[i] << 8) + (dt[i + 1]);
		sum += w;
	}

	if ((len % 2) != 0) {
		sum += (dt[len - 1] << 8);
	}

	while (sum >> 16)
		sum = (sum & 0xFFFF) + (sum >> 16);
	return (uint16_t)(~sum);
}

void
gen_header(void *buf, size_t len, uint16_t seq)
{
	memset(buf, 0, len);
	t_icmp *pkt = (t_icmp*)(buf);

	pkt->type = ICMP_ECHO;
	pkt->code = 0;
	pkt->id = htons(getpid());
	pkt->seq = seq;
	socklen_t addr_l = sizeof(struct sockaddr_in);

	pkt->checksum = htons(get_checksum(buf, len));	
}

void 
send_packet(int fd, const void *buf, size_t len,
	       	const struct sockaddr* dest_addr,
	       	socklen_t dest_len)
{	
	ssize_t r = sendto(fd, buf, len, 0, dest_addr, dest_len);
	if (r < 0)
		ft_error("ft_ping", strerror(errno));
}

float
time_calc(struct timeval* start, struct timeval* end)
{
	return (end->tv_sec - start->tv_sec) * 1000.0 + (end->tv_usec - start->tv_usec) / 1000.0;
}

void
recv_packet(t_net *net, size_t seq, int fd, size_t len, void *buf)
{
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	int opt = 1;
	setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
	struct sockaddr_in from;
	struct iovec iov;
	struct msghdr msg;
	iov.iov_base = buf;
	iov.iov_len = len;
	memset(&msg, 0, sizeof(msg));
	memset(&from, 0, sizeof(from));
	msg.msg_name = &from;
	msg.msg_namelen = sizeof(from);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;

       	struct ip *ip_hdr = (struct ip*)buf;

	int r = recvmsg(fd, &msg, 0);
	if (r < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			if (net->f_verbose)
				printf("Timeout..\n");
			net->p_lost++;
		}
		else
			ft_error("ft_ping", strerror(errno));
	}

	size_t iphlen = (ip_hdr->ip_hl * 4);
	int ttl = (ip_hdr->ip_ttl);

	t_icmp *iptr = (t_icmp *)(buf + iphlen);
	if (iptr->type == ICMP_ECHOREPLY && iptr->id == htons(getpid())) {
		if (seq == 0) {
			if (net->f_verbose) {
				fprintf(stdout, "PING %s (%s): %d data bytes, id 0x%x = %d\n",
				       	net->ad[0].addr, net->ad[0].print_ip,
				       	(len - sizeof(t_icmp)),
					 (uint16_t)(getpid() & 0xFFFF),
					 getpid());
			}
			else {
				fprintf(stdout, "PING %s (%s): %d data bytes\n",
				       	net->ad[0].addr, net->ad[0].print_ip,
				       	(len - sizeof(t_icmp)));
			}
		}
		gettimeofday(&net->t_end, NULL);
		fprintf(stdout, "%ld bytes from %s: icmp_seq=%ld ttl=%d time=%1.3f ms\n",
			       	r, net->ad[0].print_ip, seq, ttl, time_calc(&net->t_start, &net->t_end));
		net->p_succ++;
	}
	else
		gettimeofday(&net->t_end, NULL);
}

int
main(int ac, char **av)
{
	if (getuid() != 0)
		ft_error("ft_ping", "Require root permission");
	if (ac < 2)		
		ft_error("usage error", "Require destination address");

	signal(SIGINT, sigintHandler);

	isHelp(ac, av);
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
		recv_packet(&net, ntohs(seq), net.sockfd, sizeof(buf), buf);
		seq += htons(1);
		usleep(1000000);
	}
	
	fprintf(stdout, "--- %s ping statistics ---\n"
			"%ld packets transmitted, %ld packets received, %ld%% packet loss\n"
			"round-trip min/avg/max/stddev = ?/?/?/? ms\n", // STANDARD DEVIATION FORMULA
		       		net.ad[0].addr, ntohs(seq), net.p_succ, ((net.p_lost * 100) / (size_t)ntohs(seq)));
	free_struct(&net);
}
