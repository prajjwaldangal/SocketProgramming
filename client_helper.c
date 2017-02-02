#include <string.h>

#define MAX_LINE    (1000)

char newLineChar = '\n';

char * handleS()
{
	char * cap_str = "CAP";
	char * msg_comp = (char *) malloc(sizeof(char) * MAX_LINE);
	strcat(msg_comp, cap_str);
	strcat(msg_comp, &newLineChar);
	
	char * msg = (char *) malloc(sizeof(char) * MAX_LINE-5);
	printf("Enter the message: \n");
	fgets(msg, MAX_LINE, stdin);
	scanf("%s", msg);
	
	strcat(msg_comp, msg);
	strcat(msg_comp, &newLineChar);

	return msg_comp;
}

char * handleT() 
{
	char * file_str = "FILE";
	char * msg_comp = (char *) malloc(sizeof(char) * MAX_LINE);
	strcat(msg_comp, file_str);
	strcat(msg_comp, &newLineChar);

	char * msg = (char *) malloc(sizeof(char) * MAX_LINE-6);
	printf("Enter the message: \n");
	fgets(msg, MAX_LINE, stdin);
	scanf("%s", msg);

	strcat(msg_comp, msg);
	strcat(msg_comp, &newLineChar);

	return msg_comp;
}
