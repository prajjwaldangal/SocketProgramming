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

int ParseCmdLine(int argc, char *argv[], char **szAddress, char **szPort);

int main (int argc, char *argv[]) // should start from 1 -> argv
{
	// bunch of initializations
	char buffer[MAX_LINE];
	struct sockaddr_in servaddr;
	short int port;                  /*  port number               */
	char     *szAddress;             /*  Holds remote IP address   */
    char     *szPort;                /*  Holds remote port         */
    char     *endptr;
    char choice;
	char * msg = (char *) malloc (sizeof(char) * MAX_LINE);

	// memset is required to get the connection going
    memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;			     
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(ECHO_PORT);
	printf("Enter s, t, or q (lowercase): ");
	scanf("%s", &choice);
	switch (choice)
	{
		case 's':
			msg = handleS();
			break;
		case 't':
			msg = handleT();
			break;
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
	// referenced from helper.c
	Writeline(conn_s, buffer, MAX_LINE);

	Readline(conn_s, buffer, MAX_LINE);
	
	printf("Receive buffer %s\n", buffer);
	
	return 0;
}

