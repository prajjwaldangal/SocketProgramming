#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#include <string.h>
// char * handleS()
// {
// 	return "ap";
// }

// char * handleT() 
// {

// 	return "ap";
// }

#define ECHO_PORT       (2002)
#define MAX_LINE		(1000)


int main () 
{
	char choice;
	char *msg;

	// printf("Enter s, t, or q (lowercase):\n");
	// scanf("%s", choice);

	// switch (choice)
	// {
	// 	case 's':
	// 		msg = handleS();
	// 	case 't':
	// 		msg = handleT();
	// 	case 'q':
	// 		return 0;
	// 	default:
	// 		break;
	// }

	char buffer[MAX_LINE-1];
	struct sockaddr_in servaddr;
	short int port;                  /*  port number               */
	char     *szAddress;             /*  Holds remote IP address   */
    char     *szPort;                /*  Holds remote port         */
	
	// struct hostent *h;

    memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(ECHO_PORT);

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

	Writeline(conn_s, buffer, MAX_LINE-1);
	Readline(conn_s, buffer, MAX_LINE-1);
	
	close (conn_s);
	// no bind required for client
	return 0;
}



