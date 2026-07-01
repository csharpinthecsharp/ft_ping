#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

typedef struct s_net {
	bool f_verbose;
	size_t len_addrs;
	char **addrs;
} t_net;

void ft_error(const char* title, const char* s);
void ft_help(void);

#endif
