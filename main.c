#include "header.h"

static void findAddr(char **dest, const char* src) {
	if (!strcmp("-v", src))
		return;
	(*dest) = strdup(src);
}

static size_t get_addrs_size(size_t len, char **srcs) {
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

static void parse_args (t_net *net, int ac, char **av) {	
	size_t i = 1;
	size_t j = 0;
	for (int i = 0; i < net->len_addrs; i++) {
		net->ad[i].addr = NULL;
		net->ad[i].len_addr = 0;
		net->ad[i].print_ip = NULL;
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

static void isHelp(size_t len, char **srcs) {
	for (size_t i = 1; i < len; i++) {
		if (!strcmp(srcs[i], "-?"))
			ft_help();
	}
}

static void init_struct(t_net *net) {
	net->ad = NULL;
	net->f_verbose = false;
	net->len_addrs = 0;
	net->sockfd = -1;
}

int main(int ac, char **av) {
	if (getuid() != 0)
		ft_error("ft_ping", "Require root permission");
	if (ac < 2)		
		ft_error("usage error", "Require destination address");
	isHelp(ac, av);
	t_net net;
	init_struct(&net);
	net.len_addrs = get_addrs_size(ac, av);
	net.ad = malloc(sizeof(t_addr) * (net.len_addrs + 1));
	if (!net.ad)
		ft_error("malloc", "failed to allocate addrs f[main.c]");
	parse_args(&net, ac, av);
	printf("verbose: %d\n", net.f_verbose, net.f_verbose);
	for (int i = 0; i < net.len_addrs; i++) {
		printf("addr[%d]: %s\n", i, net.ad[i].addr);
	}
	net.sockfd = create_socket();
	get_ping_infos(&net);
	//create packet;
	//Sendto (packet);
	//recvfrom (receive reply);
	free_struct(&net);
}
