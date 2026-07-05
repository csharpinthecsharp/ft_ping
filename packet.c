#include "header.h"

void 
send_packet(int fd, const void *buf, size_t len,
	       	const struct sockaddr* dest_addr,
	       	socklen_t dest_len)
{	
	ssize_t r = sendto(fd, buf, len, 0, dest_addr, dest_len);
	if (r < 0)
		ft_error("ft_ping", strerror(errno));
}

void
recv_packet(t_net *net, size_t seq, int fd, size_t len, void *buf)
{
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	int opt = 1;
	setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
	struct sockaddr_in from;
	struct iovec iov;
	struct msghdr msg;
	iov.iov_base = buf;
	iov.iov_len = len;
	memset(&msg, 0, sizeof(msg));
	memset(&from, 0, sizeof(from));
	msg.msg_name = &from;
	msg.msg_namelen = sizeof(from);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	struct ip *ip_hdr = (struct ip*)buf;

	int r = recvmsg(fd, &msg, 0);
	if (r < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
			if (net->f_verbose)
				printf("Timeout..\n");
			net->p_lost++;
		}
		else
			ft_error("ft_ping", strerror(errno));
	}

	size_t iphlen = (ip_hdr->ip_hl * 4);
	int ttl = (ip_hdr->ip_ttl);

	t_icmp *iptr = (t_icmp *)(buf + iphlen);
	if (iptr->type == ICMP_ECHOREPLY && iptr->id == htons(getpid())) {
		if (seq == 0) {
			if (net->f_verbose) {
				fprintf(stdout, "PING %s (%s): %d data bytes, id 0x%x = %d\n",
				       	net->ad[0].addr, net->ad[0].print_ip,
				       	(len - sizeof(t_icmp)),
					 (uint16_t)(getpid() & 0xFFFF),
					 getpid());
			}
			else {
				fprintf(stdout, "PING %s (%s): %d data bytes\n",
				       	net->ad[0].addr, net->ad[0].print_ip,
				       	(len - sizeof(t_icmp)));
			}
		}
		gettimeofday(&net->t_end, NULL);
		fprintf(stdout, "%ld bytes from %s: icmp_seq=%ld ttl=%d time=%1.3f ms\n",
			       	r, net->ad[0].print_ip, seq, ttl, time_calc(&net->t_start, &net->t_end));
		net->p_succ++;
	}
	else
		gettimeofday(&net->t_end, NULL);
}



