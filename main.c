#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <err.h>
#include <fcntl.h>
#include <sys/sysinfo.h>

double getCPULoadAvg() {
    int FileHandler;
    char FileBuffer[1024];
    float load;

    FileHandler = open("/proc/loadavg", O_RDONLY);
    if (FileHandler < 0) {
        return -1;
    }
    read(FileHandler, FileBuffer, sizeof(FileBuffer) - 1);
    sscanf(FileBuffer, "%f", &load);
    close(FileHandler);
    return (double) load;
}

int getMemAttr(char *attr) {
    char *base = ": %d kB";
    char *pattern = malloc(strlen(attr) + strlen(base));
    sprintf(pattern, "%s%s", attr, base);

    FILE *stream = fopen("/proc/meminfo", "r");
    if (stream == NULL) return -1;

    char *line = malloc(512);
    while (fgets(line, 50, stream)) {
        int attrValue;
        if (sscanf(line, pattern, &attrValue)) {
            fclose(stream);
            free(line);
            free(pattern);
            return attrValue;
        }
    }
    free(line);
    free(pattern);
    fclose(stream);
    return -1;
}

int main() {
    int one = 1, client_fd;
    struct sockaddr_in svr_addr, cli_addr;
    socklen_t sin_len = sizeof(cli_addr);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) err(1, "can't open socket");
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));

    int port = 8080;
    svr_addr.sin_family = AF_INET;
    svr_addr.sin_addr.s_addr = INADDR_ANY;
    svr_addr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) == -1) {
        close(sock);
        err(1, "Can't bind");
    }

    listen(sock, 5);

    for (;;) {
        client_fd = accept(sock, (struct sockaddr *) &cli_addr, &sin_len);

        char buf[512];
        if (recv(client_fd, buf, 512, 0) < 0) {
            // user disconnected or timeout (if you set a timeout)
            // NO call to exit; use "continue" or "return", or something else
            // to gracefully handle the break;
            continue;
        }

        char *response = malloc(1024);
        sprintf(response,
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: application/json; charset=UTF-8\r\n\r\n"
                "{\"load_avg\":%f, "
                "\"ram_size\":%d, "
                "\"ram_usage\":%d"
                "}",
                getCPULoadAvg(),
                getMemAttr("MemTotal") / 1000,
                getMemAttr("MemTotal") / 1000 - getMemAttr("MemAvailable") / 1000);
        int i = 0;
        for (; i < strlen(response) && i != '}'; i++) {}
        char *resp = malloc(i + 1);
        memcpy(resp, response, i);
        write(client_fd, resp, i);
        close(client_fd);
    }
}