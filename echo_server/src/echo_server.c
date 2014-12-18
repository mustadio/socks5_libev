#include <ev.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#include "echo_server.h"
#include "utils.h"

#define BUF_SIZE 2048


typedef struct echo_server_ctx{
    ev_io io;
    struct echo_server *server;
}echo_server_ctx;

typedef struct echo_server{
    int service_fd;
    char buf[BUF_SIZE];
    int buf_index;
    size_t len;
    echo_server_ctx recv_ctx;
    echo_server_ctx send_ctx;
}echo_server;



int new_echo_server(EV_P_ int service_fd)
{
    echo_server *service = NULL;
    if((service = malloc(sizeof(echo_server))) == NULL) {
        LOGE("malloc error");
        return -1;
    }
    memset(service, 0, sizeof(echo_server));
    
    service->service_fd = service_fd;
    service->recv_ctx.server = service;
    service->send_ctx.server = service;
    
    ev_io_init(&service->recv_ctx.io, server_recv_cb, service_fd, EV_READ);
    ev_io_init(&service->send_ctx.io, server_send_cb, service_fd, EV_WRITE);

    ev_io_start(EV_A_ &service->recv_ctx.io);
    return 0;
}

static void close_echo_server(EV_P_ echo_server *service)
{
    if (NULL != service) {
        ev_io_stop(EV_A_ &service->recv_ctx.io);
        ev_io_stop(EV_A_ &service->send_ctx.io);
        close(service->service_fd);
        free(service);
    }
}

void server_recv_cb (EV_P_ ev_io *w, int revents)
{
    echo_server_ctx *recv_ctx = CONTAINER_OF(w, echo_server_ctx, io);
    /* echo_server_ctx *recv_ctx = (echo_server_ctx *) w; */
    echo_server *service = recv_ctx->server;
    ssize_t r = recv(service->service_fd, service->buf, BUF_SIZE, 0);
    if (r <= 0) {
        /* TODO:  */
        LOGD("remote close");
        close_echo_server(EV_A_ service);
        return;
    }
    service->buf_index = 0;
    service->len = r;
    ev_io_stop(EV_A_ &service->recv_ctx.io);
    ev_io_start(EV_A_ &service->send_ctx.io);
    
    return;
}

void server_send_cb(EV_P_ ev_io *w, int revents)
{
    echo_server_ctx *send_ctx = CONTAINER_OF(w, echo_server_ctx, io);
    /* echo_server_ctx *send_ctx = (echo_server_ctx *) w; */
    echo_server *service = send_ctx->server;
    ssize_t s = send(service->service_fd, service->buf + service->buf_index,
                     service->len, 0);
    if (s < 0) {
        LOGE("send return error");
        close_echo_server(EV_A_ service);
        /* TODO:  */
    }
    else if (s < service->len) {
        service->buf_index += s;
        service->len -= s;
    }
    else {
        ev_io_stop(EV_A_ &service->send_ctx.io);
        ev_io_start(EV_A_ &service->recv_ctx.io);
    }
    return;
}


