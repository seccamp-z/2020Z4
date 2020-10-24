
## SRv6設定の実践

hint
```
ip -6 route add 1::/128 encap seg6local action End dev eth0
ip -6 route add 1::/128 encap seg6local action End.X nh6 2:: dev eth0
ip -6 route add 1::/128 encap seg6local action End.DX4 nh4 10.0.0.1 dev eth0
```
