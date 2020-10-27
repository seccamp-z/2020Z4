#ifndef _SECCAMP_NETLINK_TYPE_H_
#define _SECCAMP_NETLINK_TYPE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <unistd.h>
#include <sys/socket.h>
#include <asm/types.h>
#include <arpa/inet.h>

#include <netinet/in.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/genetlink.h>

static void
dump_nlmsghdr(FILE *fp, const struct nlmsghdr *n)
{
	fprintf(fp, "len:   %u\n", n->nlmsg_len  );
	fprintf(fp, "type:  %u\n", n->nlmsg_type );
	fprintf(fp, "flags: %u\n", n->nlmsg_flags);
	fprintf(fp, "seq:   %u\n", n->nlmsg_seq  );
	fprintf(fp, "pid:   %u\n", n->nlmsg_pid  );
}

static void
dump_rtmsg(FILE *fp, const struct rtmsg *r)
{
	fprintf(fp, "family:   %u\n", r->rtm_family  );
	fprintf(fp, "dst_len:  %u\n", r->rtm_dst_len );
	fprintf(fp, "src_len:  %u\n", r->rtm_src_len );
	fprintf(fp, "tos:      %u\n", r->rtm_tos     );
	fprintf(fp, "table:    %u\n", r->rtm_table   );
	fprintf(fp, "protocol: %u\n", r->rtm_protocol);
	fprintf(fp, "scope:    %u\n", r->rtm_scope   );
	fprintf(fp, "type:     %u\n", r->rtm_type    );
	fprintf(fp, "flags:    %u\n", r->rtm_flags   );
}

static void
dump_rtattr(FILE *fp, const struct rtattr *a)
{
	fprintf(fp, "type=%u, ", a->rta_type);
	fprintf(fp, "len=%u, ", a->rta_len );
	fprintf(fp, "value=0x");
	const uint8_t *data = (void*)(a+1);
	for (size_t i=0; i<(a->rta_len-4); i++) {
		fprintf(fp, "%02x", data[i]);
	}
	fprintf(fp, "\n");
}

#endif /* _SECCAMP_NETLINK_TYPE_H_ */
