#include <stdlib.h>
#include <ev.h>

#include "utils.h"
#include "server_common.h"
#include "echo_server.h"



int main(int argc, char *argv[])
{
    int listen_fd = getLocalListenFd("8888");
    if (listen_fd < 0)
        exit(-1);

    struct ev_loop *loop = ev_default_loop(0);
    if (!loop) {
        LOGE("ev_loop error");
        exit(-1);
    }
    ev_io *listener_io = new_listener(listen_fd, new_echo_server);
    ev_io_init(listener_io, accept_cb, listen_fd, EV_READ);
    ev_io_start(loop, listener_io);
    ev_run (loop, 0);
    return 0;
}
