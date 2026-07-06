#ifndef STRUCT_H
#define STRUCT_H

#include "header.h"

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
	float ms_min;
	float ms_max;
	float ms_total;
	float ms_total2;
} t_net;

#endif
