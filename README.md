# Lmon

Lightweight Linux monitoring

## Getting Started

You can receive node metrics by HTTP

### Exapmle

> curl http://127.0.0.1:8080/

```
{
  "info": {
    "hostname": "Dragon-PC",
    "kernel": "5.0.13-zen1-1-zen"
  },
  "cpu": {
    "load_avg": 2.38
  },
  "memory": {
    "size": 12217,
    "usage": 6164,
    "swap": {
      "size": 8388,
      "usage": 0
    }
  },
  "drives": [
    {
      "partition": "/dev/sda5",
      "mountPoint": "/",
      "size": 48,
      "usage": 34
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

