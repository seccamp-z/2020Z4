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
  char router_id[256];
  struct neighbor *neighbors[MAX_NEIGH];
  struct network *networks[MAX_NETWORK];
};

struct neighbor {
  char address[256];
};

struct network {
  char prefix[256];
};

static inline void
print_confg(const struct config* cfg)
{
  printf("router-id: %s\n", cfg->router_id);
  for (size_t i = 0; i < MAX_NEIGHBOR; i++) {
    if (!cfg->networks[i])
      continue;
    printf("neighbor[%d]: %s\n", i, cfg->neighbors[i]);
  }
  for (size_t i = 0; i < MAX_NETWORK; i++) {
    if (!cfg->networks[i])
      continue;
    printf("network[%d]: %s\n", i, cfg->networks[i]);
  }
}
```

上記の機能をlibraryのように使えるようにしよう.
以下のsample codeのように動く config.h を実装してください
```c
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

## 課題:
- message formatは?
- etc.

## 実装
- 言語は?
- 設定ファイルは?
