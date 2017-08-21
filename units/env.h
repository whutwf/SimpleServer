#ifndef CLIENT_H__
#define CLIENT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <signal.h>

#define LISTENQ     1024
#define MAXLINE     4096

/**------------server env------------*/
#define SERV_PORT   9877

typedef void Sigfunc(int);
Sigfunc *signal(int signo, Sigfunc *func);

#ifdef __cplusplus
}
#endif
#endif