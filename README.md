# Lmon 

[![Build Status](https://drone.oceancraft.ru/api/badges/Yak/lmon/status.svg)](https://drone.oceancraft.ru/Yak/lmon)

## Getting Started

You can just receive node metrics by HTTP

### Build
Standart build with cmake and make

```
cmake -S .
make
```

### How it works

Launch agent:

> lmon agent

Launch server:

> lmon server

Get data by http:

> curl http://127.0.0.1:8080/

```
{
  "info": {
    "hostname": "Dragon-PC",
    "ip_address": "192.168.1.14",
    "uptime": 9352,
    "system": {
      "name": "Linux",
      "release": "5.0.13-zen1-1-zen",
      "version": "#1 ZEN SMP PREEMPT Sun May 5 18:05:42 UTC 2019",
      "arch": "x86_64"
    }
  },
  "cpu": {
    "processors_count": 4,
    "load_avg": 2.26,
    "processes_count": 754
  },
  "memory": {
    "size": 11931,
    "usage": 6319,
    "shared": 452,
    "buffer": 234,
    "swap": {
      "size": 8191,
      "usage": 0
    }
  },
  "drives": [
    {
      "partition": "/dev/sda5",
      "mount_point": "/",
      "size": 48,
      "usage": 35
    },
    {
      "partition": "/dev/sda6",
      "mount_point": "/home",
      "size": 191,
      "usage": 147
    }
  ]
}
```

