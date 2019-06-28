# Lmon 

Lightweight monitoring

## Build

Dependencies: libcurl, jansson, libhttpd and [libbrd](https://git.oceancraft.ru/yak/libbrd)

```
cmake -S .
make
```

## Usage

### Cluster

##### 1) Run agent on each node.
```
lmon agent
```

##### 2) Run the server on some node.
```
lmon server
```

##### 3) Wait a few seconds for the server node to start the service discovery.
```
INFO  server.c:35: added agent ('http://IP:PORT')
```
##### 4) Connect to the server node and get metrics from all nodes.
```
curl 'http://IP_OR_NAME:PORT'
```
### Single node
```
lmon agent
```