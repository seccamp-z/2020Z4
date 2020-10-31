# 便利コマンドたち

## Netns Link add を簡単にやる

多分みんなこうやってやる.
```
ip netns add ns0
ip netns add ns1
ip -n ns0 link set lo up
ip -n ns1 link set lo up
ip link add veth0 type veth peer name veth1
ip link add veth1 type veth peer name veth2
ip link set veth0 netns ns0
ip link set veth1 netns ns1
ip -n ns0 link set veth0 name net0
ip -n ns1 link set veth1 name net0
```

実はこんなやり方がある.

```bash
ip netns add ns0
ip netns add ns1
ip -n ns0 link set lo up
ip -n ns1 link set lo up
ip link add net0 netns ns0 type veth peer net0 netns ns1
```

```bash
# ip -n ns0 -br link
lo               UNKNOWN        00:00:00:00:00:00 <LOOPBACK,UP,LOWER_UP>
net0@if2         DOWN           22:3d:1a:e0:7e:a9 <BROADCAST,MULTICAST>

# ip -n ns1 -br link
lo               UNKNOWN        00:00:00:00:00:00 <LOOPBACK,UP,LOWER_UP>
net0@if2         DOWN           86:3a:46:da:d3:31 <BROADCAST,MULTICAST>
```
