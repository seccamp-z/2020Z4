# IP Routing Software DEMO

## 例題 topo4 with BGP

ここではIP RoutingのDemonstrationとしてBGPを用いたroutingを紹介します.
実際にtopo4のnetworkを構成します.

R1 configuration
```
router bgp 1
 bgp router-id 1.1.1.1
 neighbor 10.255.1.2 remote-as 2
 neighbor 10.255.2.2 remote-as 3
 neighbor 10.255.4.2 remote-as 4

 address-family ipv4 unicast
  network 10.1.0.0/24
  network 10.2.0.0/24
 exit-address-family
```

R2 configuration
```
router bgp 2
 bgp router-id 2.2.2.2
 neighbor 10.255.1.1 remote-as 1
 neighbor 10.255.3.2 remote-as 3
 neighbor 10.255.5.2 remote-as 4

 address-family ipv4 unicast
  network 10.3.0.0/24
  network 10.4.0.0/24
 exit-address-family
```

R3 configuration
```
router bgp 3
 bgp router-id 3.3.3.3
 neighbor 10.255.2.1 remote-as 1
 neighbor 10.255.3.1 remote-as 2
```

R4 configuration
```
router bgp 4
 bgp router-id 4.4.4.4
 neighbor 10.255.4.1 remote-as 1
 neighbor 10.255.5.1 remote-as 2
```

## 動作確認
- 動作確認(1) un1, un3は接続可能かどうか.
- 動作確認(2) cn1を切断した場合どうなるか.
- 実際の経路はどうなっているか.
- R3,4のRIBはどうなっているか.

## 用語の説明
- RIB: Routing Information Base
- FIB: Forwarding Information Base
- 経路広報
