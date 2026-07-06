#include "../include/header.h"

void
check_help(size_t len, char **srcs)
{
	for (size_t i = 1; i < len; i++) {
		bool marked = false;
		if (srcs[i][0] == '-' && (strlen(srcs[i]) > 1))
			marked = true;
		if (!strcmp(srcs[i], "-?")
				|| !strcmp(srcs[i], "--help"))
			help();
		else if (!strcmp(srcs[i], "--usage"))
			sh_help();
		if (marked &&
			(strcmp("-v", srcs[i]) &&
			 strcmp("--verbose", srcs[i])))
			err_help("invalid option --", srcs[i]);
	}
}

void
init_struct(t_net *net)
{
	net->ad = NULL;
	net->f_verbose = false;
	net->len_addrs = 0;
	net->sockfd = -1;
	net->p_lost = 0;
	net->p_succ = 0;
	net->wrong_host_mltp = false;
	net->ms_min = 0.0f;
	net->ms_max = 0.0f;
	net->ms_total = 0.0f;
	net->ms_total2 = 0.0f;
}

void
find_addr(char **dest, const char* src)
{
	if (!strcmp("-v", src) ||
			(!strcmp("--verbose", src)))
		return;
	(*dest) = strdup(src);
}

size_t
get_addrs_size(size_t len, char **srcs)
{
	size_t i = 1;
	size_t count = 0;
	while (i < len) {
		if (!strcmp("-v", srcs[i]) ||
				!strcmp("--verbose", srcs[i])) {
			i++;
			continue;
		}
		i++;
		count++;
	}
	return (count);
}

void
parse_args (t_net *net, size_t ac, char **av)
{	
	size_t i = 1;
	size_t j = 0;
	for (size_t i = 0; i < net->len_addrs; i++) {
		net->ad[i].addr = NULL;
		net->ad[i].len_addr = 0;
		net->ad[i].print_ip = NULL;
		net->ad[i].ip = NULL;
	}

	while (i < ac) {
		if (!strcmp("-v", av[i]) ||
				!strcmp("--verbose", av[i])) {
			net->f_verbose = true;
	       		i++;
		}
		if (j < net->len_addrs) {
			find_addr(&net->ad[j].addr, av[i]);
			j++;
		}
		i++;
	}
	if (net->len_addrs == 0)
		ft_error("usage error", "Require destination address");
}
