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
	int n, f, connfd, sock[100], read_flag;
	int no = 0;
	int ok = 1;
	pid_t pid;
	char buf[INET_ADDRSTRLEN];
	char buf1[INET6_ADDRSTRLEN];
	char readBuff[256];
	char sendBuff[256];

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;

	if( getaddrinfo("login2.sekiya-lab.info", "31601", &hints, &res0) != 0){
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

		if (setsockopt(sock[n], SOL_SOCKET, SO_REUSEADDR, (const char*)&ok, sizeof(ok)) < 0) {
			perror("setsockopt SO_REUSEADDR");
			exit(EXIT_FAILURE);
		}

		if ( res->ai_family == AF_INET) {
			printf("ipv4 address : %s\n", inet_ntop(res->ai_family, &((struct sockaddr_in *)(res->ai_addr))->sin_addr, buf, INET_ADDRSTRLEN));
		} else if ( res->ai_family == AF_INET6) {
			if (setsockopt(sock[n], IPPROTO_IPV6, IPV6_V6ONLY, (const char*)&no, sizeof(no)) < 0) {
				perror("setsockopt IPV6_V6ONLY");
				exit(EXIT_FAILURE);
			}
			printf("ipv6 address : %s\n", inet_ntop(res->ai_family, &((struct sockaddr_in6 *)(res->ai_addr))->sin6_addr, buf1, INET6_ADDRSTRLEN));
		}
		
		if (bind(sock[n], res->ai_addr, res->ai_addrlen) < 0) {
			perror("bind");
			exit(EXIT_FAILURE);
		}

		if ( (pid = fork() ) == 0 ) {
			if (listen(sock[n], LISTENQ) < 0) {
				perror("listen");
				exit(EXIT_FAILURE);
			}
			while(1) {
				connfd = accept(sock[n], (struct sockaddr*)NULL, NULL); 
				/*				
				ticks = time(NULL);
				snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
				write(connfd, sendBuff, strlen(sendBuff)); 
				*/
				write(connfd, "HELLO\n", strlen("HELLO\n")); 
				memset(readBuff,	0, sizeof(readBuff));
				while(read_flag =	read(connfd,	readBuff,	sizeof(readBuff))){
					if (read_flag > 0) {
						printf("read: %s\n", readBuff);
					} else {
						perror("read");
						exit(EXIT_FAILURE);
					}
					write(connfd, readBuff, strlen(readBuff)); 
				}
				//close(connfd);
				sleep(1);
			}
		}
			
		res = res->ai_next;
		n++;
	}

	freeaddrinfo(res0);
	return 0;
}
