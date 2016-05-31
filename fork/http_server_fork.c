#include<stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define LISTENQ 10

int main(int argc, char **argv) 
{
	struct addrinfo hints, *res0, *res;
	time_t ticks;
	int n, f, connfd, sock[20];
	char *yes = "lo";
	char sendBuff[256];

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;

	if( getaddrinfo("localhost", "31601", &hints, &res0) != 0){
		herror("getaddrinfo");
		exit(EXIT_FAILURE);
	}

	res = res0;
	n = 0;

	while ( res != NULL) {
		sock[n] = socket(res->ai_family, res->ai_socktype, 0);
		if(sock[n] < 0) {
			perror("socket");
			exit(EXIT_FAILURE);
		}
		if (setsockopt(sock[n], IPPROTO_IPV6, IPV6_V6ONLY, &yes, sizeof(yes)) < 0) {
			perror("setsockopt");
			exit(EXIT_FAILURE);
		}
		if (bind(sock[n], res->ai_addr, res->ai_addrlen) < 0) {
			perror("bind");
			exit(EXIT_FAILURE);
		}
		if (listen(sock[n], LISTENQ) < 0) {
			perror("listen");
			exit(EXIT_FAILURE);
		}
		while(1) {
  		connfd = accept(sock[n], (struct sockaddr*)res->ai_addr, sizeof(res->ai_addr)); 
      
			//ticks = time(NULL);
      //snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
      snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", "hello");
      write(connfd, sendBuff, strlen(sendBuff)); 

      close(connfd);
      sleep(1);
		}
		res = res->ai_next;
		n++;
	}

	freeaddrinfo(res0);
	return 0;
}
