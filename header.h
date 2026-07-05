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
#include <signal.h>
#include <sys/time.h>
#include <netinet/ip.h>
typedef struct s_icmp {
	uint8_t type;
	uint8_t code;
	uint16_t checksum;
	uint16_t id;
	uint16_t seq;
} t_icmp;

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
	size_t p_lost;
	size_t p_succ; 
	struct timeval t_start;
	struct timeval t_end;
	bool wrong_host_mltp;
} t_net;


void ft_error(const char* title, const char* s);
void help(void);
void sh_help(void);
void err_help(const char c);
void free_struct(t_net *net);
float time_calc(struct timeval* start, struct timeval* end);

int create_socket(void);
void get_ping_infos(t_net *net);

void check_help(size_t len, char **srcs);
void parse_args (t_net *net, int ac, char **av);
void find_addr(char **dest, const char* src);
size_t get_addrs_size(size_t len, char **srcs);
void init_struct(t_net *net);

void send_packet(int fd, const void *buf, size_t len, const struct sockaddr* dest_addr, socklen_t dest_len);
void recv_packet(t_net *net, size_t seq, int fd, size_t len, void *buf);

uint16_t get_checksum(void *buf, size_t len);
void gen_header(void *buf, size_t len, uint16_t seq);

#endif
