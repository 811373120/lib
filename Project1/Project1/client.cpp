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
#include <signal.h>
#include <string>
#include "./protocol/protoctest.pb.h"
int main()
{
	goprotobuf::HelloWorld hw;
	hw.set_id(5);
	hw.set_opt(2);
	std::string s;
	
	hw.set_str("hellow");
	int fd;
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in dest_addr;
	memset(&dest_addr, 0, sizeof(dest_addr));
	dest_addr.sin_addr.s_addr= inet_addr("127.0.0.1");
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_addr.s_addr = INADDR_ANY;
	dest_addr.sin_port = htons(5555);
	int ret=connect(fd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr));
	if (ret == -1) {
		printf("error");
	}
	std::string data;
	char buf[1024];
	hw.SerializeToString(&data);
	char sendbuf[data.length()];
	strcpy(sendbuf, data.c_str());
	printf("%d", sizeof(sendbuf));
	int n=send(fd, sendbuf, sizeof(sendbuf), 0);
	if (n <= 0) {
		printf("gggg");
	}
	
	close(fd);
}