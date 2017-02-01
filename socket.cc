#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>     // inet (3) functions
#include <unistd.h>

#define ECHO_PORT       (2002)
#define MAX_LINE        (1000)
#define LISTENQ			(500)

int main() 
{

	char * buffer;

	struct sockaddr_in servaddr;

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(ECHO_PORT);

	int list_s, conn_s;
	
	// create socket
	list_s = socket(AF_INET, SOCK_STREAM, 0);
	
	// bind socket to address
	bind(list_s, (struct sockaddr *) &servaddr, sizeof(servaddr));
	
	// listen
	if (listen(list_s, LISTENQ) < 0)
	{
		printf("Not listening \n");
		return 0;
	}

	while (1)
	{
		// accept
		if ((conn_s = accept(list_s, NULL, NULL)) < 0)
		{
			printf("Not accepting \n");
			break;
		}

		// read(conn_s, buffer, MAX_LINE - 1);
		Readline(conn_s, buffer, MAX_LINE-1);
		

		close (conn_s);

	}
	// sa_family -> AF_INET
	// sa_data    ->    port numbers
	
}

ssize_t Readline(int sockd, void *vptr, size_t maxlen) {
    ssize_t n, rc;
    char    c, *buffer;

    buffer = vptr;

    for ( n = 1; n < maxlen; n++ ) {
	
		if ( (rc = read(sockd, &c, 1)) == 1 ) {
		    *buffer++ = c;
		    if ( c == '\n' )
			break;
		}
		else if ( rc == 0 ) {
		    if ( n == 1 )
				return 0;
		    else
				break;
		}
		else {
		    if ( errno == EINTR )
				continue;
		    return -1;
		}
    }

    *buffer = 0;
    return n;
}

