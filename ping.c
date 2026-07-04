#include "header.h"
#define FIRST_ARG 0

int
create_socket(void)
{
	int fd;
	fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (fd < 0)
		ft_error("ft_ping", strerror(errno));
	return (fd);
}

void
get_ping_infos(t_net *net)
{
	for (size_t i = 0; i < net->len_addrs; i++) { 
		struct addrinfo hints, *res;
		
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET; // IPV4
		hints.ai_flags = AI_CANONNAME; // IP + CANONICAL IP (www,etc)
		hints.ai_flags |= AI_IDN; // handle non-ascii (gûûgle.comi)

		int rc = getaddrinfo(net->ad[i].addr, NULL, &hints, &res);
		if (rc) {
			if (i == 0) {
				free_struct(net);
				ft_error("ft_ping", "unknown host");
			}
			return;
		}
		
		net->ad[i].len_addr = res->ai_addrlen;
		net->ad[i].print_ip = malloc(sizeof(char*) * (net->ad[i].len_addr + 1));
		net->ad[i].ip = malloc(sizeof(struct sockaddr_in));
		memcpy(net->ad[i].ip, res->ai_addr, sizeof(struct sockaddr_in));
		inet_ntop(AF_INET, &(net->ad[i].ip->sin_addr), net->ad[i].print_ip, net->ad[i].len_addr);
		printf("addr: %s\naddrlen: %d\nhostname: %s\n",
			       	net->ad[i].print_ip, net->ad[i].len_addr, net->ad[i].addr);
		freeaddrinfo(res);
	}
}
