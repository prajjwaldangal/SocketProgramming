#ifndef PG_SOCK_HELP
#define PG_SOCK_HELP

#include <unistd.h>

#define LISTENQ        (1024)   /*  Backlog for listen()   */

ssize_t Readline(int sockd, void *vptr, size_t maxlen);
ssize_t Writeline(int sockd, const void *vptr, size_t n);
#endif
