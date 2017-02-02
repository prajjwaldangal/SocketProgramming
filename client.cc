#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

// #include <string.h>
#include <errno.h>

#include "client_helper.c"

#define ECHO_PORT       (2002)
#define MAX_LINE		(1000)

ssize_t Readline(int sockd, char *vptr, size_t maxlen);
ssize_t Writeline(int sockd, char *vptr, size_t n);

int main () 
{
	char buffer[MAX_LINE-1];
	struct sockaddr_in servaddr;
	short int port;                  /*  port number               */
	char     *szAddress;             /*  Holds remote IP address   */
    char     *szPort;                /*  Holds remote port         */
	

    memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(ECHO_PORT);

	char choice;
	char * msg;
	printf("Enter s, t, or q (lowercase): ");

	scanf("%s", &choice);

	switch (choice)
	{
		case 's':
			msg = handleS();
			break;
		case 't':
			msg = handleT();
		case 'q':
			return 0;
			break;
		default:
			break;
	}
	
	int conn_s;

	// create socket
	if ((conn_s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("Error creating socket \n");
	}

	// connect
	if (connect(conn_s, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
	{
		printf("Error connecting \n");
	}

	// buffer[0] = 'a';
	// buffer[1] = 'b';

	printf("Message at client line 71: %c %c \n", *msg, *(msg + 5));
	strcpy(buffer, msg);

	Writeline(conn_s, buffer, MAX_LINE-1);
	// Readline(conn_s, buffer, MAX_LINE-1);
	// printf("%c, %c\n", buffer[0], buffer[1]);
	
	close (conn_s);
	// no bind required for client
	
	return 0;
}



ssize_t Readline(int sockd, char *vptr, size_t maxlen) {
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

ssize_t Writeline(int sockd, char *vptr, size_t n) {
    size_t      nleft;
    ssize_t     nwritten;
    char *buffer;

    buffer = vptr;
    nleft  = n;

    while ( nleft > 0 ) {
	if ( (nwritten = write(sockd, buffer, nleft)) <= 0 ) {
	    if ( errno == EINTR )
			nwritten = 0;
	    else
			return -1;
	}

	nleft  -= nwritten;
	buffer += nwritten;
    }

    return n;
}


