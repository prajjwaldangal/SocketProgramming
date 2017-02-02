#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

// #include <string.h>
#include <errno.h>

#include "helper.h"
#include "helper.c"

#include "client_helper.c"

#define ECHO_PORT       (2002)
#define MAX_LINE		(1000)

int main () 
{
	char buffer[MAX_LINE];
	struct sockaddr_in servaddr;
	short int port;                  /*  port number               */
	char     *szAddress;             /*  Holds remote IP address   */
    char     *szPort;                /*  Holds remote port         */
	

    memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(ECHO_PORT);

	char choice;
	char * msg = (char *) malloc (sizeof(char) * MAX_LINE);
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

	strcpy(buffer, msg);
	printf("msg in main-clientside %s", buffer);

	Writeline(conn_s, buffer, MAX_LINE);
	Readline(conn_s, buffer, MAX_LINE);
	printf("Receive buffer %s\n", buffer);
	// printf("%c, %c\n", buffer[0], buffer[1]);
	
	// close (conn_s);
	// no bind required for client
	
	return 0;
}


