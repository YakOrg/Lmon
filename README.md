# Lmon

Lightweight Linux monitoring

## Getting Started

You can receive node metrics by HTTP

### Exapmle

> curl http://127.0.0.1:8080/

```
{
  "info": {
    "hostname": "Dragon-PC"
  },
  "cpu": {
    "load_avg": 2.62
  },
  "memory": {
    "size": 12217,
    "usage": 4176,
    "swap": {
      "size": 8388,
      "usage": 0
    }
  }
}
```

