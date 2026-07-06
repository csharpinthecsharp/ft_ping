#include "../include/header.h"

void 
send_packet(int fd, const void *buf, size_t len,
	       	const struct sockaddr* dest_addr,
	       	socklen_t dest_len)
{	
	ssize_t r = sendto(fd, buf, len, 0, dest_addr, dest_len);
	if (r < 0)
		ft_error("ft_ping", strerror(errno));
}


size_t
recv_packet(t_net *net, int fd, size_t len, void *buf)
{
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

	int r = recvmsg(fd, &msg, 0);
	if (r < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
			if (net->f_verbose)
				printf("Timeout..\n");
			net->p_lost++;
		}
		else
			ft_error("ft_ping", strerror(errno));
	}
	return (r);
}

static void
update_ms(t_net *net, float ms, size_t index)
{
	if (index == 0) {
		net->ms_max = ms;
		net->ms_min = ms;
	}
	net->ms_total += ms;
	net->ms_total2 += ms * ms;
	if (ms < net->ms_min)
	       net->ms_min = ms;
	else if (ms > net->ms_max)
		net->ms_max = ms;	
}

static void
update_pkt_loop(t_net *net, uint16_t *seq, int ttl, float ms, size_t r)
{
	gettimeofday(&net->t_end, NULL);
	ms = time_calc(&net->t_start, &net->t_end);
	fprintf(stdout, "%ld bytes from %s: icmp_seq=%d ttl=%d time=%1.3f ms\n",
	r, net->ad[0].print_ip, ntohs((*seq)), ttl, ms);
	update_ms(net, ms, ntohs((*seq)));
	net->p_succ++;
	(*seq) += ntohs(1);
	sleep(1);
}


void
handle_pkt_reply(t_net *net, void *buf, size_t len, uint16_t *seq, size_t r)
{
	struct ip *ip_hdr = (struct ip*)buf;
	size_t iphlen = (ip_hdr->ip_hl * 4);
	int ttl = (ip_hdr->ip_ttl);
	float ms = 0.0f;
	
	t_icmp *iptr = (t_icmp *)(buf + iphlen);

	if (iptr->type != ICMP_ECHOREPLY)
		return;
	if (iptr->id != htons(getpid()))
		return;

	if (ntohs(*seq) == 0) {
		if (net->f_verbose) {
			fprintf(stdout, "PING %s (%s): %ld data bytes, id 0x%x = %d\n",
				net->ad[0].addr, net->ad[0].print_ip,
				(len - sizeof(t_icmp)),
				(uint16_t)(getpid() & 0xFFFF),
				getpid());
		}
		else {
			fprintf(stdout, "PING %s (%s): %ld data bytes\n",
				net->ad[0].addr, net->ad[0].print_ip,
				(len - sizeof(t_icmp)));
		}
	}
	update_pkt_loop(net, seq, ttl, ms, r);
}
