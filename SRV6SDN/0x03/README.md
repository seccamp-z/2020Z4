# SRv6 Software Defined Networking

ベース構成は 0x02と全く同じものを用意します.

## (1) 手動設定したポリシーを標準化してください

例えば `C1 <-> [C4,5,6] / F1,F2` を考える.
```
ip netns exec R1 ip rule add prio 100 from 10.1.0.11 table 100
ip netns exec R1 ip route add 10.2.0.0/24 encap seg6 mode encap segs FF:1::1,FF:2::1,F2::10 dev net2
ip netns exec R2 ip rule add prio 100 to 10.1.0.11 table 100
ip netns exec R2 ip route add 10.1.0.0/24 encap seg6 mode encap segs FF:2::1,FF:1::1,F1::10 dev net2
```

結論は以下の標準化をする. (1) `C1 <-> C4 / F1,F2`, (2) `C1 <-> C5 / F2,F3,F1`
```
rules:

#(1)
- id: 100
  target_src: 10.1.0.11 #C1
  target_dst: 10.2.0.14 #C4
  functions: [FF:1::1, FF:2::1]
  
#(2)
- id: 200
  target_src: 10.1.0.11 #C1
  target_dst: 10.2.0.15 #C5
  functions: [FF:2::1, FF:3::1, FF:1::1]
```

## (2) ポリシーから設定を自動生成できるようにしてください

実際のRouting設定は以下のようになる.
```
#(1)
ip netns exec R1 ip rule add from 10.1.0.11 to 10.2.0.13 table 100
ip netns exec R1 ip route add 10.2.0.13/32 encap seg6 mode enca segs FF:1::1,FF:2::1,F2::10 dev net2 table 100
ip netns exec R2 ip rule add from 10.2.0.13 to 10.1.0.11 table 100
ip netns exec R2 ip route add 10.1.0.11/32 encap seg6 mode enca segs FF:2::1,FF:1::1,F1::10 dev net2 table 100

#(2)
SRC=10.1.0.11
DST=10.2.0.15
TAB=200
SID_LIST_UP="FF:2::1,FF3::1,FF:1::1"
SID_LIST_DN="FF:1::1,FF3::1,FF:2::1"

ip netns exec R1 ip rule add from $SRC to $DST table $TAB
ip netns exec R1 ip route add $DST/32 encap seg6 mode enca segs ${SID_LIST_UP},F2::10 dev netb2 table $TAB
ip netns exec R2 ip rule add from $DST to $SRC table $TAB
ip netns exec R2 ip route add $SRC/32 encap seg6 mode enca segs ${SID_LIST_DN},F1::10 dev net2 table $TAB
```

これをpythonで開発どうぞ!

## (3) 自動生成された設定から実際に通信内容が変更されるまでを動作確認してください
