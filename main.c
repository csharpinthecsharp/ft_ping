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

static void isHelp(size_t len, char **srcs) {
	for (size_t i = 1; i < len; i++) {
		if (!strcmp(srcs[i], "-?"))
			ft_help();
	}
}

int main(int ac, char **av) {
	if (ac < 2)		
		ft_error("usage error", "Require destination address");

	isHelp(ac, av);
	
	t_net net;
	net.addrs = NULL;
	net.f_verbose = false;
	net.len_addrs = 0;
	size_t i = 1;
	size_t j = 0;
	
	net.len_addrs = get_addrs_size(ac, av);
	net.addrs = malloc(sizeof(char**) * (net.len_addrs + 1));
	if (!net.addrs)
		ft_error("malloc", "failed to allocate addrs f[main.c]");

	for (int i = 0; i < net.len_addrs; i++) {
		net.addrs[i] = NULL;
	}

	while (i < ac) {
		if (!strcmp("-v", av[i])) {
			net.f_verbose = true;
	       		i++;
		}
		if (j < net.len_addrs) {
			findAddr(&net.addrs[j], av[i]);
			j++;
		}
		i++;
	}
	if (net.len_addrs == 0)
		ft_error("usage error", "Require destination address");

	printf("verbose: %d\n", net.f_verbose, net.f_verbose);

	for (int i = 0; i < net.len_addrs; i++) {
		printf("addr[%d]: %s\n", i, net.addrs[i]);
	}

	for (int i = 0; i < net.len_addrs; i++) {
		free(net.addrs[i]);
	}
	free(net.addrs);
}
