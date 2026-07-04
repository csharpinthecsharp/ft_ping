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
#include <stdint.h>
#include <netinet/ip_icmp.h>

typedef struct s_addr {
	char* addr;
	char* print_ip;
	struct sockaddr_in* ip;
	size_t len_addr;
} t_addr;

typedef struct s_net {
	bool f_verbose;
	size_t len_addrs;
	t_addr* ad;
	int sockfd;
} t_net;

void ft_error(const char* title, const char* s);
void ft_help(void);

void free_struct(t_net *net);

int create_socket(void);
int get_ping_infos(t_net *net);
#endif
