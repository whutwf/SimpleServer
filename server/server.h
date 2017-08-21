#ifndef CLIENT_H__
#define CLIENT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <error.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
// #include "env.h"


#define LISTENQ     1024
#define MAXLINE     4096

/**------------server env------------*/
#define SERV_PORT   9877

typedef void Sigfunc(int);
Sigfunc *wrap_signal(int signo, Sigfunc *func);
void sig_chld(int signo);

void str_echo(int sockfd);
int go_server_ipv4();

#ifdef __cplusplus
}
#endif
#endif