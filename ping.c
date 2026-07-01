#include "header.h"

int create_socket(void) {
	int fd;
	fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (fd < 0)
		ft_error("ft_ping", strerror(errno));
	return (fd);
}
