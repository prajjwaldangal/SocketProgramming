#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>     // inet (3) functions
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include "helper.h"
#include "helper.c"

char newLineChar = '\n';

#define ECHO_PORT       (2002)
#define MAX_LINE        (1000)
#define LISTENQ			(1024)

int main(int argc, char *argv[]) 
{

	char buffer[MAX_LINE];
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
		Readline(conn_s, buffer, MAX_LINE);
		char * cap_str = "CAP";
		char * file_str = "FILE";
		char * ret_str = (char *) malloc(sizeof(char) * MAX_LINE);
		// char ret_str[MAX_LINE-1];
		char * semi_buf = (char *) malloc(sizeof(char) * MAX_LINE-5);
		// char semi_buf[MAX_LINE-1];
		char conv[4]; // convert cap_count to char

		printf("Received buffer: %s\n", buffer);

		FILE * fp;
		char file_path[MAX_LINE-7];
		long bytes;
		char n_bytes[MAX_LINE-3]; // this means we will have limitation as to the size of file
		char file_buf[MAX_LINE-1];

		int n_cap = 0, n_file = 0;

		for (int i=0; i < 3; i++) {
			if (buffer[i] == cap_str[i]) {
				n_cap ++;
			}
			else if (buffer[i] == file_str[i]) {
				n_file ++;
			} 
			else {
				continue;
			}

		}
		printf("n_cap: %d, n_file: %d\n", n_cap, n_file);
		int cap_count;
		if (n_cap > n_file)
		{
			printf("if block\n");
			int i = 4;
			int n_c = 1;
			while (n_c < 2)
			{
				if (buffer[i] == '\n')
				{
					n_c++;
				} else {
					* (semi_buf+ i-4) = toupper(buffer[i]);
					cap_count++;
				}
				i++;	
			}
			printf("str len %lu\n", strlen(buffer));
			// for (int i=0; i < str)

			sprintf(ret_str, "%i", cap_count);  /* converting int to string */
			// sprintf(ret_str, "%s", "\n");
			// sprintf(ret_str, "%s", semi_buf);
			strcat(ret_str, "\n");
			strcat(ret_str, semi_buf);
			printf("return string %s", ret_str);
			printf("semi_buf: %s\n", semi_buf);

		} else {
			for (int i=5; i < strlen(buffer)-1; i++) { // MAX_LINE - 1 to escape reading the last \n
				file_path[i-5] = buffer[i];
			}
			printf("file path: %s", file_path);
			fp = fopen(file_path, "r");
			if (fp == NULL) {
				strcat(ret_str, "9");
				strcat(ret_str, &newLineChar);
				strcat(ret_str, "NOT FOUND");
			} else {
				if (fseek(fp, 0, SEEK_END) != 0) {
					printf("Error in seeking to the end of file");
				}
				bytes = ftell(fp);
				printf("File size %ld\n", bytes);
				sprintf(n_bytes, "%d", bytes);
				strcat(ret_str, n_bytes);
				strcat(ret_str, "\n");
				// strcat(ret_str, getchar(fp));
				// Writeline(conn_s, ret_str, MAX_LINE-1);
				// char c = fgetc
				// while ((char) fgetc(fp) != EOF) {
				// 	memset(ret_str, 0, sizeof(ret_str));
				// 	if (strlen(ret_str) == MAX_LINE-1) {
				// 		Writeline(conn_s, ret_str, MAX_LINE-1);
				// 	} else {
				// 		strcat(ret_str, getchar(fp));
				// 	}
				// }							
			}
		}
		// printf("cap count: %d, new buffer: %s\n", cap_count, semi_buf);
		Writeline(conn_s, ret_str, MAX_LINE-1);
		close (conn_s);
	}
}
