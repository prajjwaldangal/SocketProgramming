#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>     // inet (3) functions
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>


#define ECHO_PORT       (2002)
#define MAX_LINE        (1000)
#define LISTENQ			(1024)

ssize_t Readline(int sockd, char *vptr, size_t maxlen);
ssize_t Writeline(int sockd, char *vptr, size_t n);

int main() 
{

	char buffer[MAX_LINE-1];

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
		printf("MSG at server line 57: %c %c %c\n", *(buffer), *(buffer+1), *(buffer+8));

		// CAPthisisthefuture
		// corf is temporary, cf contains whether CAP or FILE
		char C_or_F[3], cf[3];

		C_or_F[0] = *buffer;
		C_or_F[1] = *(buffer+1);
		C_or_F[2] = *(buffer+2);

		// client -> server: "CAP\nxxx\n"

		// server -> client: "FILE\nxxx\n"

		strcat(cf, C_or_F);

		printf("isequal %d, string: %d %d %d %c %c %c\n", (strcmp(cf, "CAP")), (strcmp(C_or_F, "CAP")), (cf == "CAP"), (C_or_F == "CAP"), *buffer, *(buffer+1), *(buffer+2));

		int cap_count;
		if ((strcmp(C_or_F, "CAP") < 1))
		{
			int i = 3;
			int n_c = 1;
			while (n_c < 2)
			{
				if (*(buffer+i) == '\n')
				{
					n_c++;
				} else {
					*(buffer+i) = toupper(*(buffer+i));
					cap_count++;
				}
				i++;	
				printf("Cap count, server 77: %d, isequal: %d", cap_count, strcmp(cf, "CAP"));
			}
			
		}
		Writeline(conn_s, buffer, MAX_LINE-1);

		close (conn_s);

	}
	// sa_family -> AF_INET
	// sa_data    ->    port numbers
	
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



