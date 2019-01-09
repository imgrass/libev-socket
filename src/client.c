#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// ev_io_init
int main(int argc, char *argv[]) {
    struct sockaddr_in addr;
    int fd;
    int ret;
    socklen_t addrlen;
    char buffer[128];

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd <= 0) {
        fprintf(stderr, "socket build error!\n");
        exit(1);
    }

    addr.sin_family = AF_INET;
    ret = inet_aton("127.0.0.1", &addr.sin_addr); 
    if (ret != 1) {
        fprintf(stderr, "invalid id address!\n");
        exit(1);
    }
    addr.sin_port = htons(25190);

    addrlen = sizeof(struct sockaddr);
    ret = connect(fd, (struct sockaddr *)&addr, addrlen);
    if (ret != 0) {
        fprintf(stderr, "connect error!\n");
        exit(1);
    }

    strcpy(buffer, "I want to change the world!");
    send(fd, buffer, 128, 0);

    close(fd);
    return 0;
}
