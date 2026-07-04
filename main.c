#include "header.h"

static void findAddr(char **dest, const char* src) {
	if (!strcmp("-v", src))
		return;
	(*dest) = strdup(src);
}

static size_t get_addrs_size(size_t len, char **srcs) {
	size_t i = 1;
	size_t count = 0;
	while (i < len) {
		if (!strcmp("-v", srcs[i])) {
			i++;
			continue;
		}
		i++;
		count++;
	}
	return (count);
}

static void parse_args (t_net *net, int ac, char **av) {	
	size_t i = 1;
	size_t j = 0;
	for (int i = 0; i < net->len_addrs; i++) {
		net->ad[i].addr = NULL;
		net->ad[i].len_addr = 0;
		net->ad[i].print_ip = NULL;
	}

	while (i < ac) {
		if (!strcmp("-v", av[i])) {
			net->f_verbose = true;
	       		i++;
		}
		if (j < net->len_addrs) {
			findAddr(&net->ad[j].addr, av[i]);
			j++;
		}
		i++;
	}
	if (net->len_addrs == 0)
		ft_error("usage error", "Require destination address");
}

static void isHelp(size_t len, char **srcs) {
	for (size_t i = 1; i < len; i++) {
		if (!strcmp(srcs[i], "-?"))
			ft_help();
	}
}

static void init_struct(t_net *net) {
	net->ad = NULL;
	net->f_verbose = false;
	net->len_addrs = 0;
	net->sockfd = -1;
}

uint16_t get_checksum(void *buf, size_t len) {
	uint8_t *dt = (uint8_t*)buf;
	uint32_t sum = 0;

	for (size_t i = 0; i < len; i+=2) {
		uint16_t w = (dt[i] << 8) + (dt[i + 1]);
		sum += w;
	}

	if ((len % 2) != 0) {
		sum += (dt[len - 1] << 8);
	}

	while (sum >> 16)
		sum = (sum & 0xFFFF) + (sum >> 16);
	return (uint16_t)(~sum);
}

int main(int ac, char **av) {
	if (getuid() != 0)
		ft_error("ft_ping", "Require root permission");
	if (ac < 2)		
		ft_error("usage error", "Require destination address");
	isHelp(ac, av);
	t_net net;
	init_struct(&net);
	net.len_addrs = get_addrs_size(ac, av);
	net.ad = malloc(sizeof(t_addr) * (net.len_addrs + 1));
	if (!net.ad)
		ft_error("malloc", "failed to allocate addrs f[main.c]");
	parse_args(&net, ac, av);
	printf("verbose: %d\n", net.f_verbose, net.f_verbose);
	for (int i = 0; i < net.len_addrs; i++) {
		printf("addr[%d]: %s\n", i, net.ad[i].addr);
	}
	net.sockfd = create_socket();
	get_ping_infos(&net);
	//create packet;
	//Sendto (packet);
	//PING LOOP {
	struct icmp_header {
		uint8_t type; //0x08 (8) for echo request
		uint8_t code; //0 for basic ping request
		uint16_t checksum; //Unique ID 
		uint16_t identifier; //Track if packet #1 arrived before 2 or if 2 swas lost
		uint16_t sequence; //16 bit handle with htons();
	};
	//each packet need a header
	uint8_t buf[64];
	memset(buf, 0, sizeof(buf));
	struct icmp_header *pkt = (struct icmp_header *)buf;


	// !!!!!!!!!!!!!!!!!!!!!!!!!!!! SEQUENCE INCREMENT A CHAQUE NOUVEL ENVOI	
	pkt->type=ICMP_ECHO;
	pkt->code=0;
	pkt->identifier=htons(getpid());
	pkt->sequence=htons(0);
	socklen_t addr_l = sizeof(struct sockaddr_in);

	pkt->checksum = htons(get_checksum(buf, sizeof(buf)));
	ssize_t b = sendto(net.sockfd,
		       	buf,
		       	sizeof(buf),
		       	0,
		       	(struct sockaddr *)net.ad[0].ip,
		       	sizeof(struct sockaddr_in));
	if (b < 0)
		ft_error("ft_ping", strerror(errno));
	printf("Sent %zd bytes\n", b);

	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	setsockopt(net.sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

	struct sockaddr_in from;
	struct iovec iov;
	struct msghdr msg;
	struct cmsghdr *cmsg;
	char cmsg_data[1024];
	iov.iov_base = buf;
	iov.iov_len = sizeof(buf);
	memset(&from, 0, sizeof(from));
	msg.msg_name = &from;
	msg.msg_namelen = sizeof(from);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = cmsg_data;
	msg.msg_controllen = sizeof(cmsg_data);
	int r = recvmsg(net.sockfd, &msg, 0);
	if (r < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			printf("Timeout..\n");
		else
			ft_error("ft_ping", strerror(errno));
	}
	// EXTRAIRE TTL ENSUITE QUAND CA MARCHE
	struct icmp_header *iptr = (struct icmp_header*)(buf + 20);
	// ECHOREPLY when fixed
	if (iptr->type == ICMP_ECHOREPLY && iptr->identifier == htons(getpid()))
		printf("Receiv %zd bytes\n", r);
	free_struct(&net);
}
