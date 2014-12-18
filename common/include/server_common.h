#ifndef SERVER_COMMON_H
#define SERVER_COMMON_H

typedef int (*create_server_fp)(EV_P_ int service_fd);

ev_io* new_listener(int listen_fd, create_server_fp init_fp);
void accept_cb (EV_P_ ev_io *w, int revents);
int getLocalListenFd(const char *port);

#endif /* SERVER_COMMON_H */
