#ifndef HEADER_H
#define HEADER_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <arpa/inet.h>

typedef struct s_net {
	bool f_verbose;
	size_t len_addrs;
	char **addrs;
	int sockfd;
} t_net;

void ft_error(const char* title, const char* s);
void ft_help(void);

int create_socket(void);
#endif
