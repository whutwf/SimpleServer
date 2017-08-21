#ifndef CLIENT_H__
#define CLIENT_H__

#ifdef __cplusplus
extern "C" {
#endif


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <error.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <math.h>

#define MAXLINE     4096

/**------------server env------------*/
#define SERV_PORT   9877

void str_cli(int sockfd, FILE *fp);
int go_client_ipv4(int port, char *ip_name);


#ifdef __cplusplus
}
#endif
#endif