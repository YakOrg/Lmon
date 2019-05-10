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

Launch lmon:

> lmon 8080

Get data by http:

> curl http://127.0.0.1:8080/

```
{
  "info": {
    "hostname": "Dragon-PC",
    "ip": "192.168.14.248",
    "kernel": "5.0.13-zen1-1-zen"
  },
  "cpu": {
    "load_avg": 2.07
  },
  "memory": {
    "size": 12217,
    "usage": 5839,
    "swap": {
      "size": 8388,
      "usage": 0
    }
  },
  "drives": [
    {
      "partition": "/dev/sda6",
      "mountPoint": "/home",
      "size": 48,
      "usage": 35
    },
    {
      "partition": "/dev/sda6",
      "mountPoint": "/home",
      "size": 191,
      "usage": 148
    }
  ]
}
```

