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

		/* printf("----\n"); */
		/* dump_nlmsghdr(stdout, &msg->n); */
		if (msg->n.nlmsg_type != RTM_NEWROUTE &&
		    msg->n.nlmsg_type != RTM_GETROUTE &&
		    msg->n.nlmsg_type != RTM_DELROUTE) {
			printf("it's not route message\n");
		}

		/* printf("----\n"); */
		/* dump_rtmsg(stdout, &msg->r); */

		printf("----\n");
		printf("[scope] = %x\n", msg->r.rtm_scope);
		struct rtattr *tb[100];
		size_t all_attr_len = msg->n.nlmsg_len
			- sizeof(struct nlmsghdr)
			- sizeof(struct rtmsg);

		parse_rtattr(tb, 100,
			(struct rtattr*)msg->buf,
			all_attr_len);

		if (tb[RTA_DST]) {
			char str[256];
			uint32_t addr = rta_getattr_u32(tb[RTA_DST]);
			inet_ntop(AF_INET, &addr, str, sizeof(str));
			printf("RTA_DST %s\n", str);
			tb[RTA_DST] = NULL;
		}

		if (tb[RTA_GATEWAY]) {
			char str[256];
			uint32_t addr = rta_getattr_u32(tb[RTA_GATEWAY]);
			inet_ntop(AF_INET, &addr, str, sizeof(str));
			printf("RTA_GATEWAY %s\n", str);
			tb[RTA_GATEWAY] = NULL;
		}


		if (tb[RTA_OIF]) {
			uint32_t val = rta_getattr_u32(tb[RTA_OIF]);
			printf("RTA_OIF %u\n", val);
			tb[RTA_OIF] = NULL;
		}

		if (tb[RTA_TABLE]) {
			uint32_t val = rta_getattr_u32(tb[RTA_TABLE]);
			printf("RTA_TABLE %u\n", val);
			tb[RTA_TABLE] = NULL;
		}

		// FOR_DEBUGGING
		/* for (size_t i=0; i<100; i++) */
		/* 	if (tb[i]) */
		/* 		printf("found i=%zd\n", i); */

		ptr0 = (ptr0 + msg->n.nlmsg_len);
	}
	return;
}

static void list_routes(int fd)
{
	struct {
		struct nlmsghdr n;
		struct rtmsg r;
		char buf[4096];
	} req = {
		.n.nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg)),
		.n.nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST,
		.n.nlmsg_type = RTM_GETROUTE,
		.r.rtm_family = AF_INET,
	};

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
	list_routes(fd);
	close(fd);
}
