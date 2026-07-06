#include "../include/header.h"

uint16_t
get_checksum(void *buf, size_t len)
{
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

void
gen_header(void *buf, size_t len, uint16_t seq)
{
	memset(buf, 0, len);
	t_icmp *pkt = (t_icmp*)(buf);

	pkt->type = ICMP_ECHO;
	pkt->code = 0;
	pkt->id = htons(getpid());
	pkt->seq = seq;
	pkt->checksum = htons(get_checksum(buf, len));
}
