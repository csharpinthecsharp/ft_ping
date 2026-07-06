#ifndef PROTOTYPE_H
#define PROTOTYPE_H

#include "header.h"

void ft_error(const char* title, const char* s);
void help(void);
void sh_help(void);
void err_help(const char* s, const char* c);
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
size_t recv_packet(t_net *net, size_t seq, int fd, size_t len, void *buf);
void handle_pkt_reply(t_net *net, void *buf, size_t len, uint16_t *seq, size_t r);

uint16_t get_checksum(void *buf, size_t len);
void gen_header(void *buf, size_t len, uint16_t seq);

#endif
