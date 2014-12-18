#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <ev.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

#include "utils.h"
#include "server_common.h"

typedef struct listen_ctx{
    ev_io io;
    int listen_fd;
    create_server_fp init_fp;
}listen_ctx;

ev_io* new_listener(int listen_fd, create_server_fp init_fp)
{
    listen_ctx *listener = malloc(sizeof(listen_ctx));
    if (NULL == listener) {
        LOGE("malloc listener fail");
        return NULL;
    }
    memset(listener, 0, sizeof(listen_ctx));
    listener->listen_fd = listen_fd;
    listener->init_fp = init_fp;
    return &(listener->io);
}

void accept_cb (EV_P_ ev_io *w, int revents)
{
    listen_ctx *listener = CONTAINER_OF(w, listen_ctx, io);
    /* listen_ctx *listener = (listen_ctx *)w; */
    struct sockaddr_in cliaddr = {0};
    socklen_t len = sizeof(cliaddr);
    int service_fd = accept(listener->listen_fd, (struct sockaddr*)&cliaddr, &len);
    if (-1 == service_fd) {
        LOGE("accept error");
        return;
    }
    LOGD("client addr is %s", inet_ntoa(cliaddr.sin_addr));

    if (-1 == listener->init_fp(EV_A_ service_fd)) {
        close(service_fd);
    }

    /* int opt = 1; */
    /* setsockopt(service_fd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt)); */
    return;
}

int getLocalListenFd(const char *port)
{
    struct addrinfo hints;
    struct addrinfo *results, *rp;
    struct sockaddr_in *h;
    int listen_fd = -1;
    int ret;

    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if((ret = getaddrinfo(NULL, port, &hints, &results)) != 0) {
        LOGE("Can't get listen addr");
        return -1;
    }

    for(rp = results; rp != NULL; rp = rp->ai_next) {
        h = (struct sockaddr_in *) rp->ai_addr;
        listen_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (-1 == listen_fd)
            continue;
        
        int opt = 1;
        setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        setsockopt(listen_fd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));

        if(0 == bind(listen_fd, rp->ai_addr, rp->ai_addrlen)) {
            /* LOGD("local addr is %s, port is %d", inet_ntoa(h->sin_addr), ntohs(h->sin_port)); */
            break;
        }
        close(listen_fd);
    }

    if (NULL == rp) {
        LOGE("Can't bind any addr");
        listen_fd = -1;
    }
    else {
        if (0 != listen(listen_fd, 10)) {
            LOGE("listen at fd fail");
            listen_fd = -1;
        }
    }
    
    freeaddrinfo(results);
    return listen_fd;
}
