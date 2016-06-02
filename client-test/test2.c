#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>

#define PORT 31601

void talk_ipv4_server(char* server_add)
{
	int sock;
	int add_binder;
	int read_flag;
	int write_flag;
	int conn_flag;
	char buf[256];
	char sendBuff[256];
	struct sockaddr_in server;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	printf("Socket descriptor: %d\n", sock);
	if(sock < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	server.sin_family = AF_INET;
	server.sin_port =	htons(PORT);

	add_binder = inet_pton(AF_INET, server_add, &server.sin_addr);
	printf("Address translate flag: %d\n", add_binder);
	if(add_binder < 1) {
		perror("address");
		exit(EXIT_FAILURE);
	}

	conn_flag = connect(sock,	(struct sockaddr *)&server,	sizeof(server));
	printf("Connection flag: %d\n", conn_flag);
	if(conn_flag < 0) {
		perror("connect");
		exit(EXIT_FAILURE);
	}

	while(1) {
		memset(buf,	0, sizeof(buf));
		read_flag =	read(sock,	buf,	sizeof(buf));
		if(read_flag < 0) {
			perror("read");
			exit(EXIT_FAILURE);
		}
		printf("get: %s\n", buf);
		scanf("%s", sendBuff);
		write(sock, sendBuff, strlen(sendBuff)); 
		if(read_flag < 0) {
			perror("write");
			exit(EXIT_FAILURE);
		}
		//sleep(1);
	}

	close(sock);
}

void talk_ipv6_server(char* server_add)
{
	int sock;
	int add_binder;
	int read_flag;
	int write_flag;
	int conn_flag;
	char buf[256];
	char sendBuff[256];
	struct sockaddr_in6 server;

	sock = socket(AF_INET6, SOCK_STREAM, 0);
	printf("Socket descriptor: %d\n", sock);
	if(sock < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	server.sin6_family = AF_INET6;
	server.sin6_port =	htons(PORT);

	add_binder = inet_pton(AF_INET6, server_add, &server.sin6_addr);
	printf("Address translate flag: %d\n", add_binder);
	if(add_binder < 1) {
		perror("address");
		exit(EXIT_FAILURE);
	}

	conn_flag = connect(sock,	(struct sockaddr *)&server,	sizeof(server));
	printf("Connection flag: %d\n", conn_flag);
	if(conn_flag < 0) {
		perror("connect");
		exit(EXIT_FAILURE);
	}

	while(1) {
		memset(buf,	0, sizeof(buf));
		read_flag =	read(sock,	buf,	sizeof(buf));
		if(read_flag < 0) {
			perror("read");
			exit(EXIT_FAILURE);
		}
		printf("get: %s\n", buf);
		scanf("%s", sendBuff);
		write(sock, sendBuff, strlen(sendBuff)); 
		if(read_flag < 0) {
			perror("write");
			exit(EXIT_FAILURE);
		}
		//sleep(1);
	}

	close(sock);
}

int main(int argc, char **argv)
{
	char server_add_ipv6[40];
	char server_add_ipv4[20];

	if(strcmp(argv[1], "4") == 0) {
		strcpy(server_add_ipv4, argv[2]);
		printf("Server address: %s (ipv4)\n", server_add_ipv4);
		talk_ipv4_server(server_add_ipv4);
	} else if(strcmp(argv[1], "6") == 0) {
		strcpy(server_add_ipv6, argv[2]);
		printf("Server address: %s (ipv6)\n", server_add_ipv6);
		talk_ipv6_server(server_add_ipv6);
	} else {
		printf("Wrong address type. Type 4 for ipv4 or 6 for ipv6\n");
	}

	return 0;
}
