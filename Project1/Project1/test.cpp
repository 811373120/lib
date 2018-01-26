#include <stdio.h>
#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <sys/queue.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <errno.h>
struct event_base* evbase;

void buffered_on_read(struct bufferevent *bev, void *arg);
void buffered_on_error(int fd, short ev, void *arg);
void on_accept(int fd, short ev, void *arg);
int setnonblock(int fd) {
	int flags = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	return 1;
}
struct client {
	/* The clients socket. */
	int fd;

	/* The bufferedevent for this client. */
	struct bufferevent *buf_ev;
	/*
	* This holds the pointers to the next and previous entries in
	* the tail queue.
	*/
	TAILQ_ENTRY(client) entries;
};
TAILQ_HEAD(, client) client_tail_head;
void
on_accept(int fd, short ev, void *arg)
{
	int client_fd;
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	struct client *client;

	client_fd = accept(fd, (struct sockaddr *)&client_addr, &client_len);
	if (client_fd < 0) {
		//printf("accept failed");
		return;
	}

	/* Set the client socket to non-blocking mode. */
	if (setnonblock(client_fd) < 0)
		printf("failed to set client socket non-blocking");

	/* We've accepted a new client, create a client object. */
	client = (struct client *)calloc(1, sizeof(*client));
	if (client == NULL)
		printf("malloc failed");
	client->fd = client_fd;

	client->buf_ev = bufferevent_socket_new(evbase, client_fd, 0);
	bufferevent_setcb(client->buf_ev, buffered_on_read, NULL,
		NULL, client);

	/* We have to enable it before our callbacks will be
	* called. */
	bufferevent_enable(client->buf_ev, EV_READ);

	/* Add the new client to the tailq. */
	TAILQ_INSERT_TAIL(&client_tail_head, client, entries);

	printf("Accepted connection from %s\n",
		inet_ntoa(client_addr.sin_addr));
}
void
buffered_on_read(struct bufferevent *bev, void *arg)
{
	struct client *this_client = (struct client *)arg;
	struct client *client;
	uint8_t data[8192];
	size_t n;

	/* Read 8k at a time and send it to all connected clients. */
	for (;;) {
		n = bufferevent_read(bev, data, sizeof(data));
		if (n <= 0) {
			/* Done. */
			break;
		}
		/* Send data to all connected clients except for the
		* client that sent the data. */

		TAILQ_FOREACH(client, &client_tail_head, entries) {
			if (client != this_client) {
				bufferevent_write(client->buf_ev, data, n);
			}
		}

	}
}
int main()
{
	int listen_fd;
	struct sockaddr_in listen_addr;
	struct event ev_accept;
	int reuseaddr_on;

	/* Initialize libevent. */
	evbase = event_base_new();

	/* Initialize the tailq. */
	TAILQ_INIT(&client_tail_head);

	/* Create our listening socket. */
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd < 0)
		printf("listen failed");
	memset(&listen_addr, 0, sizeof(listen_addr));
	listen_addr.sin_family = AF_INET;
	listen_addr.sin_addr.s_addr = INADDR_ANY;
	listen_addr.sin_port = htons(5555);

	if (bind(listen_fd, (struct sockaddr *)&listen_addr,
		sizeof(listen_addr)) < 0)
		printf("bind failed");
	if (listen(listen_fd, 5) < 0)
		printf("listen failed");
	reuseaddr_on = 1;
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr_on,
		sizeof(reuseaddr_on));
	111
	/* Set the socket to non-blocking, this is essential in event
	* based programming with libevent. */
	if (setnonblock(listen_fd) < 0)
		printf("failed to set server socket to non-blocking");

	/* We now have a listening socket, we create a read event to
	* be notified when a client connects. */
	event_assign(&ev_accept, evbase, listen_fd, EV_READ | EV_PERSIST,
		on_accept, NULL);
	event_add(&ev_accept, NULL);

	/* Start the event loop. */
	event_base_dispatch(evbase);



	return 0;
}