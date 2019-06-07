# Lmon 
[![Build Status](https://drone.oceancraft.ru/api/badges/Yak/lmon/status.svg)](https://drone.oceancraft.ru/Yak/lmon)

Lightweight monitoring

## Setup and usage

We wrote a systemd service setup script for x86_64 distros.

#### 1) Install the agent on each node.
```
curl -s 'https://get.oceancraft.ru/lmon.sh' | sh -s agent
```

#### 2) Install the server on some node.
```
curl -s 'https://get.oceancraft.ru/lmon.sh' | sh -s server
```

#### 3) Magic... Wait a few seconds for the server node to start the service discovery.
```
INFO  server.c:35: added agent ('http://IP:PORT')
```
#### 4) Connect to the server node and get metrics from all nodes.
```
curl 'http://IP_OR_NAME:PORT'
```
## Single node usage
```
lmon agent
```