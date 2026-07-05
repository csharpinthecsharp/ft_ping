#include "header.h"

void
ft_error(const char* title, const char* s)
{
	fprintf(stderr, "%s: %s\n", title, s);
	exit(1);
}

void
help(void)
{
	fprintf(stdout, "Usage: ./ft_ping [OPTION...] HOST...\n "
			"SEND ICMP ECHO_REQUEST packets to network hosts.\n\n "
			"Options valid for all request types:\n\n "
			" -v, --verbose		verbose output\n\n"
			"Options valid for --echo requests:\n\n "
			" -?, --help		give this help list\n"
			"      --usage		give a short usage message\n");
	exit(1);
}

void
sh_help(void)
{
	fprintf(stdout, "Usage: ./ft_ping [-v?]\n"
			"[--verbose]\n"
			"[--help] [--usage]\n"
			"HOST...\n");
	exit(1);
}

void
err_help(const char c)
{
	fprintf(stdout, "ft_ping: invalid option -- '%c'\n"
			"Try './ft_ping --help' or './ft_ping --usage' for more information.\n", c);
	exit(1);
}

float
time_calc(struct timeval* start, struct timeval* end)
{
	return (end->tv_sec - start->tv_sec) * 1000.0 + (end->tv_usec - start->tv_usec) / 1000.0;
}

void
free_struct(t_net *net)
{
	if (net) {
		for (int i = 0; i < net->len_addrs; i++) {
			if (net->ad[i].addr)
				free(net->ad[i].addr);
			if (net->ad[i].print_ip)
				free(net->ad[i].print_ip);
			if (net->ad[i].ip)
				free(net->ad[i].ip);
		}
	}
	if (net->ad)
		free(net->ad);
	close(net->sockfd);
}

