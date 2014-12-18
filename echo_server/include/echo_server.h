#ifndef ECHO_SERVER_H
#define ECHO_SERVER_H

int new_echo_server(EV_P_ int service_fd);
void server_recv_cb (EV_P_ ev_io *w, int revents);
void server_send_cb (EV_P_ ev_io *w, int revents);


#endif /* ECHO_SERVER_H */
