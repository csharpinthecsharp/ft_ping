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
		net->addrs[i] = NULL;
	}

	while (i < ac) {
		if (!strcmp("-v", av[i])) {
			net->f_verbose = true;
	       		i++;
		}
		if (j < net->len_addrs) {
			findAddr(&net->addrs[j], av[i]);
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
	net->addrs = NULL;
	net->f_verbose = false;
	net->len_addrs = 0;
	net->sockfd = -1;
}

static void free_struct(t_net *net) {
	for (int i = 0; i < net->len_addrs; i++) {
		free(net->addrs[i]);
	}
	free(net->addrs);
	close(net->sockfd);
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
	net.addrs = malloc(sizeof(char**) * (net.len_addrs + 1));
	if (!net.addrs)
		ft_error("malloc", "failed to allocate addrs f[main.c]");

	parse_args(&net, ac, av);

	printf("verbose: %d\n", net.f_verbose, net.f_verbose);
	for (int i = 0; i < net.len_addrs; i++) {
		printf("addr[%d]: %s\n", i, net.addrs[i]);
	}
	
	net.sockfd = create_socket();
	

	for (size_t i = 0; i < net.len_addrs; i++) { 
		struct addrinfo hints, *res;
		
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET; // IPV4
		hints.ai_flags = AI_CANONNAME; // IP + CANONICAL IP (www,etc)
		hints.ai_flags |= AI_IDN; // handle non-ascii (gûûgle.comi)
						//
		int rc = getaddrinfo(net.addrs[i], NULL, &hints, &res);

		char ip_print[res->ai_addrlen];
		struct sockaddr_in *ip = (struct sockaddr_in *)res->ai_addr;
		inet_ntop(AF_INET, &(ip->sin_addr), ip_print, res->ai_addrlen);
		printf("addr: %s\naddrlen: %d\nhostname: %s\ncannonname: %s\n",
			       	ip_print, res->ai_addrlen, net.addrs[i], res->ai_canonname);
		freeaddrinfo(res);
	}
	free_struct(&net);
}
