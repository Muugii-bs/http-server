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
  char buf[INET_ADDRSTRLEN];
  char buf1[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_family = AF_UNSPEC;

	printf("input: %s\n", argv[1]);
	if( getaddrinfo(NULL, "80", &hints, &res0) != 0){
		herror("getaddrinfo");
		return 0;
	}

	res = res0;

	while ( res != NULL) {
		if ( res->ai_family == AF_INET) {
			printf("here1\n");
			printf("ipv4 address : %s\n", inet_ntop(res->ai_family, &((struct sockaddr_in *)(res->ai_addr))->sin_addr, buf, INET_ADDRSTRLEN));
		} else if ( res->ai_family == AF_INET6) {
			printf("here2\n");
			printf("ipv6 address : %s\n", inet_ntop(res->ai_family, &((struct sockaddr_in6 *)(res->ai_addr))->sin6_addr, buf1, INET6_ADDRSTRLEN));
		}
		res = res->ai_next;
	}
	freeaddrinfo(res0);
	return 0;
}
