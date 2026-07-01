#include "header.h"

void ft_error(const char* title, const char* s) {
	fprintf(stderr, "%s: %s\n", title, s);
	exit(1);
}

void ft_help(void) {
	fprintf(stderr, "Options:\n-v\n-?\n");
	exit(1);
}
