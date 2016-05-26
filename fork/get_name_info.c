#include<stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char **argv) 
{
	struct addrinfo hints, *res0, *res;
	char buff[NI_MAXSERV];

	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_family = AF_UNSPEC;
	getaddrinfo(argv[1], NULL, &hints, &res0);
	res = res0;
	while ( res != NULL) {
		getnameinfo(res->ai_addr, res->ai_addrlen, buff, sizeof(buff), NULL, 0, NI_NUMERICHOST);
		printf("addr: %s\n", buff);
		res = res->ai_next;
	}
	freeaddrinfo(res0);
	return 0;
}
