# Routing Softwareを作ってみよう

## 課題: ちゃんと設定ファイルを読み込めるようにしよう

以下のファイルを読み取れるようにします.
```json
{
  "router-id": "1.1.1.1",
  "neighbors": [
    { "address": "10.255.1.1" }
  ],
  "networks": [
    { "prefix": "10.3.0.0/24" },
    { "prefix": "10.4.0.0/24" }
  ]
}
```

c言語の以下の構造体に設定を保存します.
```c
#define MAX_NEIGH 32
#define MAX_NETWORK 256

struct config {
  struct in_addr router_id;
  struct neighbor *neighbors[MAX_NEIGH];
  struct network *networks[MAX_NETWORK];
};

struct neighbor {
  struct in_addr address;
};

struct network {
  struct prefix prefix;
};

struct prefix {
  struct in_addr addr;
  uint32_t length;
};

static inline void
print_confg(const struct config* cfg)
{
  char addr_str[256];
  inet_ntop(AF_INET, &cfg->router_id, addr_str, 256);
  printf("router-id: %s\n", addr_str);
  
  for (size_t i = 0; i < MAX_NEIGHBOR; i++) {
    if (!cfg->networks[i])
      continue;
    inet_ntop(AF_INET, &cfg->neighbors[i].address, addr_str, 256);
    printf("neighbor[%d]: %s\n", i, addr_str);
  }
  
  for (size_t i = 0; i < MAX_NETWORK; i++) {
    if (!cfg->networks[i])
      continue;
    inet_ntop(AF_INET, &cfg->networks[i].prefix.addr, 
              addr_str, 256);
    printf("network[%d]: %s\n", i, addr_str, 
           cfg->networks[i].prefix.length);
  }
}
```

上記の機能をlibraryのように使えるようにしよう.
以下のsample codeのように動く config.h を実装してください
```c
#include <stdlib.h>
#include <stdout.h>
#include "config.h"

int main()
{
  config cfg;
  int ret = config_parse(&cfg, "./r1.json");
  if (ret < 0) {
    fprintf(stderr, "failed on config parser\n");
    return 1;
  }
  
  print_config(&cfg);
}
```

## 課題: 経路広報できるようにしよう

この課題はtopo2で動作させます.
R1(server), R2(clien)として動作させます.

この課題の目標は, 接続先に対して自身の接続しているnetwork情報を通知することです.
プログラムが動作した結果, R1は `10.3.0.0/24`, `10.4.0.0/24` の情報,
R2は`10.1.0.0/24`, `10.2.0.0/24` の情報をそれぞれ対向のRouterから受け取れるように
プログラムを書き換えます.

実際に送受信をするメッセージフォーマットを以下にc言語の構造体として定義します.
これをベースにコードを書いてください.

```c
#define MAX_HOP 32

#define MSG_TYPE_UNSPEC
#define MSG_TYPE_UPDATE
#define MSG_TYPE_WITHDRAW

struct message {
  uint32_t type; // MSG_TYPE_XX
  struct in_addr path[MAX_HOP];
  struct prefix networks[MAX_NETWORK];
};
```

TCPの接続後には, それぞれのconfig fileに設定された `networks` の内容を
接続先のrouterに, `struct message` の形式で送信してください.

情報を受け取った方のRouterは, その内容を標準出力に書き込んでください.
以下のような形式で表示してみてください

(R1の場合)
```
type=UPDATE, path=[1.1.1.1], network={10.3.0.0/24}
type=UPDATE, path=[1.1.1.1], network={10.4.0.0/24}
```
