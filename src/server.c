/**
 * this is a server.
 * socket addr is 127.0.0.1:25190
 * based on io event
 */
#include <ev.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ERR2STR (strerror(errno))

struct lwlb_server {
    int fds_server;
    int fds_client;
    struct sockaddr addr_server;
    struct sockaddr addr_client;
};

static void init_server(struct lwlb_server *lwlb) {
    struct sockaddr_in addr;
    int fd;
    char address[64];
    unsigned short port = 25190;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    strcpy(address, "127.0.0.1");
    if (inet_pton(AF_INET, address, &addr.sin_addr.s_addr) != 1) {
        fprintf(stderr, "%s is invalid\n", address);
        exit(1);
    }
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        fprintf(stderr, "socket call failed!\n");
        exit(1);
    }

    if(bind(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr)) != 0)
        fprintf(stderr, "bind %d error: %s!\n",
                fd, strerror(errno));

    if(listen(fd, 10) != 0) {
        fprintf(stderr, "listen %d error: %s!\n",
                fd, strerror(errno));
        exit(1);
    }

    lwlb->fds_server = fd;
    lwlb->addr_server = *(struct sockaddr *)&addr;
}

static void do_server(struct lwlb_server *lwlb) {
    socklen_t sock_len;
    char buffer[128];

    sock_len = sizeof(struct sockaddr);
    lwlb->fds_client = accept(lwlb->fds_server, 
                              &lwlb->addr_client,
                              &sock_len); 
    if (lwlb->fds_client < 0) {
        fprintf(stderr, 
                "----accept error!----\n"
                "lwlb->fds_server=%d\n"
                "lwlb->fds_client=%d\n"
                "error is %s\n",
                lwlb->fds_server, 
                lwlb->fds_client,
                ERR2STR
                );
        exit(1);
    }

    int ret;
    ret = recv(lwlb->fds_client, buffer, 128, 0); 
    if (ret <= 0) {
        fprintf(stderr, "recv error!: %s\n", ERR2STR);
        exit(1);
    }
    fprintf(stdout, "I recv %s \n", buffer);

    strcpy(buffer, "ok, I recieved it!");
    fprintf(stdout, "and I'll send %s \n", buffer);
    ret = send(lwlb->fds_client, buffer, 128, 0);
    if (ret <= 0) {
        fprintf(stderr, "send error!: %s\n", ERR2STR);
        exit(1);
    }
}

static void release_lwlb(struct lwlb_server *lwlb) {
    close(lwlb->fds_client);
    close(lwlb->fds_server);
    free(lwlb);
}

int main(int argc, char *argv[]) {
    struct lwlb_server *lwlb;

    lwlb = (struct lwlb_server *)calloc(1, sizeof(struct lwlb_server));
    if (lwlb == NULL) {
        fprintf(stderr, "calloc lwlb_server error!\n");
        exit(1);
    }

    init_server(lwlb);

    do_server(lwlb);

    release_lwlb(lwlb);

    return 0;
}
