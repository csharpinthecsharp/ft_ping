// ping adress
// ping -v address
// ping -? address
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define TRUE 0
#define FALSE 1
#define EDGE_CASE 2

static int isFlag(const char *s) {	
	if (*s != '-')
		return (FALSE);
	s++;
	while (*s) {
		if (*s != '?' && *s != 'v')
			return (EDGE_CASE);
		s++;		
	}
	return (TRUE);
}

static void ft_error(const char* title, const char* s) {
	fprintf(stderr, "%s: %s\n", title, s);
	exit(1);
}

static void ft_help(void) {
	fprintf(stderr, "Options:\n-v\n-?\n");
	exit(1);
}

int main(int ac, char **av) {
	(void)av;
	if (ac < 2)
		ft_error("usage error", "Require destination address");
	if (ac == 2) {
		if (isFlag(av[1]) == EDGE_CASE)
			ft_help();
		else if (isFlag(av[1]) == TRUE)
			ft_error("usage error", "Require destination address");
		else
			return (0);
	}
	if (ac > 2) .....
	return (0);	
}
