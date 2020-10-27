# Linux Networking Configuration with Netlink

## Netlinkとは..?

## iproute2をbuildしてみよう

```
```

## iproute2のroute list を見てみよう

- ip route
	- `ip/iproute.c:iproute_list_flush_or_save()`
		- `lib/libnetlink.c:rtnl_routedump_req()`

## route listを実装してみよう

## route add

- (1) `ip route add 1.1.1.1/32 dev eth0`
  - `msg.r.rtm_scope = 0xfd` にする必要あり.
	- attrs: `RTA_DST, RTA_OIF`
- (2) `ip route add 1.1.1.1/32 via 10.0.0.2 dev eth0`
  - `msg.r.rtm_scope = 0x00` にする必要あり.
	- attrs: `RTA_DST, RTA_OIF, RTA_GATEWAY`
