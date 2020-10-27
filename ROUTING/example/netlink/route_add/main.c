#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "netlink.h"

static void parse(const void* ptr, size_t len)
{
	const uint8_t *ptr0 = (const uint8_t*)ptr;
	for (;;) {
		const struct {
			struct nlmsghdr n;
			struct rtmsg r;
			char buf[4096];
		} *msg = (const void *)ptr0;

		if (msg->n.nlmsg_type == NLMSG_ERROR ||
		    msg->n.nlmsg_type == NLMSG_DONE ||
		    msg->n.nlmsg_type == 0) {
			printf("DONE\n");
			break;
		}

		ptr0 = (ptr0 + msg->n.nlmsg_len);
	}
	return;
}

static void adddel_route(int fd, const char *dststr, const char *gwstr, uint32_t oif, bool is_add)
{
	struct {
		struct nlmsghdr n;
		struct rtmsg r;
		char buf[4096];
	} req = {
		.n.nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg)),
		.n.nlmsg_flags = NLM_F_REQUEST | NLM_F_CREATE | NLM_F_EXCL | NLM_F_ACK,
		.n.nlmsg_type = is_add ? RTM_NEWROUTE : RTM_DELROUTE,
		.r.rtm_family = AF_INET,
		.r.rtm_dst_len = 32,
		.r.rtm_src_len = 0,
		.r.rtm_tos = 0,
		.r.rtm_table = RT_TABLE_MAIN,
		.r.rtm_protocol = RTPROT_STATIC,
		.r.rtm_scope = RT_SCOPE_UNIVERSE, // FOR_VIA_ROUTE
		.r.rtm_type = RTN_UNICAST,
		.r.rtm_flags = 0,
	};

	/* set RTA_DST */
	struct in_addr prefix;
	inet_pton(AF_INET, dststr, &prefix);
	addattr_l(&req.n, sizeof(req), RTA_DST, &prefix, sizeof(struct in_addr));

	/* set RTA_GATEWAY */
	struct in_addr gw;
	inet_pton(AF_INET, gwstr, &gw);
	addattr_l(&req.n, sizeof(req),
		  RTA_GATEWAY, &gw,
		  sizeof(struct in_addr));

	/* set RTA_OIF */
	uint32_t oif_idx = oif;
	addattr32(&req.n, sizeof(req), RTA_OIF, oif_idx);

	/* set RTA_TABLE */
	uint32_t table = 254;
	addattr32(&req.n, sizeof(req), RTA_TABLE, table);

	/* submit request */
	char buf[10000];
	struct nlmsghdr *answer = (struct nlmsghdr*)buf;
	int ret = nl_talk(fd, &req.n, answer, sizeof(buf));
	if (ret < 0)
		exit(1);

	parse(answer, sizeof(buf));
}


int main(int argc, char** argv)
{
	int fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
	adddel_route(fd, "2.2.2.2", "10.231.160.1", 2, true);
	close(fd);
}
