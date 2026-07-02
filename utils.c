#include "header.h"

void ft_error(const char* title, const char* s) {
	fprintf(stderr, "%s: %s\n", title, s);
	exit(1);
}

void ft_help(void) {
	fprintf(stderr, "Usage: ping [OPTION...] HOST...\n "
			"SEND ICMP ECHO_REQUEST packets to network hosts.\n\n "
			"Options valid for all request types:\n\n "
			" -v, --verbose		verbose output\n\n"
			"Options valid for --echo requests:\n\n "
			" -?, --help		give this help list\n"
			"      --usage		give a short usage message\n");
	exit(1);
}
