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
#include <signal.h>
#include <string.h>

#define LISTENQ 100

void signal_handler (int sig) 
{
	int status, retval;
	do {
	 retval = waitpid(-1, &status, WNOHANG);
	} while (retval > 0);
	signal(SIGCHLD, signal_handler);
}

char * reply (char * text)
{	
	char *pos;
	char *ret;
	int i = 1;
	pos = strtok(text, "DUUHNAA=");
	while (pos != NULL) {
		if (i == 2) {
			ret = pos;
		}
		pos = strtok (NULL, "DUUHNAA=");
		i++;
	}
	return ret;
}

int main(int argc, char **argv) 
{
	struct addrinfo hints, *res0, *res;
	time_t ticks;
	int n, f, connfd, sock[100], read_flag, no = 0, ok = 1, status;
	pid_t pid;
	char buf[INET_ADDRSTRLEN];
	char buf1[INET6_ADDRSTRLEN];
	char readBuff[256];
	char *tmp;
	char sendBuff[1024];

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;

	if( getaddrinfo("login2.sekiya-lab.info", "31601", &hints, &res0) != 0){
		herror("getaddrinfo");
		//exit(EXIT_FAILURE);
	}

	res = res0;
	n = 0;

	while ( res != NULL) {
		sock[n] = socket(res->ai_family, res->ai_socktype, 0);
		if(sock[n] < 0) {
			perror("socket");
			//exit(EXIT_FAILURE);
		}

		if (setsockopt(sock[n], SOL_SOCKET, SO_REUSEADDR, (const char*)&ok, sizeof(ok)) < 0) {
			perror("setsockopt SO_REUSEADDR");
			//exit(EXIT_FAILURE);
		}

		if ( res->ai_family == AF_INET) {
			printf("ipv4 address : %s\n", inet_ntop(res->ai_family, &((struct sockaddr_in *)(res->ai_addr))->sin_addr, buf, INET_ADDRSTRLEN));
		} else if ( res->ai_family == AF_INET6) {
			if (setsockopt(sock[n], IPPROTO_IPV6, IPV6_V6ONLY, (const char*)&no, sizeof(no)) < 0) {
				perror("setsockopt IPV6_V6ONLY");
				//exit(EXIT_FAILURE);
			}
			printf("ipv6 address : %s\n", inet_ntop(res->ai_family, &((struct sockaddr_in6 *)(res->ai_addr))->sin6_addr, buf1, INET6_ADDRSTRLEN));
		}
		
		if (bind(sock[n], res->ai_addr, res->ai_addrlen) < 0) {
			perror("bind");
			//exit(EXIT_FAILURE);
		}
		signal (SIGCHLD, signal_handler); 
		if ((pid = fork()) == 0) {
			if (listen(sock[n], LISTENQ) < 0) {
				perror("listen");
				//exit(EXIT_FAILURE);
			}
			signal (SIGCHLD, signal_handler); 
			while(1) {
				connfd = accept(sock[n], (struct sockaddr*)NULL, NULL); 
				if ((pid = fork()) == 0) {
					close(sock[n]);
					memset(sendBuff, 0, sizeof(sendBuff));
					snprintf(sendBuff, sizeof(sendBuff), 
							"HTTP/1.0 200 OK\r\n"
							"Content-Type: text/html\r\n"
							"\r\n"
							"<font folor=red><h1>Hello World</h1></font>\r\n"
							"<form action=\"http://login2.sekiya-lab.info:31601\" method=\"post\">"
							"Name <input type=\"text\" name=\"DUUHNAA\"><br>"
							"<input type=\"submit\" value=\"Submit\"></form>\r\n");
					write(connfd, sendBuff, (int)strlen(sendBuff)); 
					memset(readBuff,	0, sizeof(readBuff));
					while(read_flag =	read(connfd,	readBuff,	sizeof(readBuff))){
						if (read_flag > 0) {
							write(connfd, "<font color=\"red\">I got your message </font>", strlen("<font color=\"red\">I got your message </font>")); 
							memset(readBuff,	0, sizeof(readBuff));
						} else {
							perror("read");
							//exit(EXIT_FAILURE);
						}
					}
					exit(0);
				} else if (pid > 0) {
					close(connfd);
				} else if (pid < 0) {
					perror("fork child");
					//exit(EXIT_FAILURE);
				}
			}
		} else if (pid > 0 ) {
			res = res->ai_next;
			n++;
		} else {
			perror("fork parent");
			//exit(EXIT_FAILURE);
		}
	}
	freeaddrinfo(res0);
	wait(&status);
	return 0;
}
