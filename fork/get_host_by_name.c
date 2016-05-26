#include<stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>

int main(int argc, char **argv) 
{
	struct hostent* hp;
	char *sin_addr;
	int i;

	printf("input: %s\n", argv[1]);
	if( (hp = gethostbyname( argv[1] )) == NULL ){
		herror("gethostname");
		return 0;
	}

	printf("h_name = %s\n", hp->h_name );
	printf("h_addrtype = %d\n", hp->h_addrtype );
	printf("h_length = %d\n", hp->h_length );

	for( i=0; hp->h_aliases[i]; i++ ){
		printf("h_aliases[%d] = %s\n", i, hp->h_aliases[i] );
	}

	for( i=0; hp->h_addr_list[i]; i++ ){
		bcopy( hp->h_addr_list[i], &sin_addr, hp->h_length );
		printf("h_addr_list[%d] = %s\n", i, (char *)inet_ntoa(sin_addr) );
	}
	return 0;
}
