#include "server.h"

int main(int argc, char **argv)
{

}

void str_echo(int sockfd)
{
    ssize_t n;
    char buf[MAXLINE];

again:
    //当服务器关闭时候，返回0
    while ((n = read(sockfd, buf, MAXLINE)) > 0) {
        if (write(sockfd, buf, n) != n) {
            fprintf(stdout, "[server] Info：write to client not full\n");
        }
    }

    if (n < 0 && errno == EINTR) {
        goto again;
    } else if (n < 0) {
        perror("[server] Error: read from client\n");
    }
}

int go_server_ipv4(int port)
{
    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        perror("[server] Error:socket begin\n");
        return -1;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = port;

    if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("[server] Error: socket bind\n");
        return -1;
    }
    if (listen(listenfd, LISTENQ) < 0) {
        perror("[server] Error: socket listen\n");
        return -1;
    }

    for ( ; ; ) {
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
        if (connfd < 0) {
            perror("[server] Error: socket connect\n");
            return -1;
        }

        if ((childpid = fork()) == 0) {
            close(listenfd);
            str_echo(connfd);
            close(connfd);
        }

        //资源计数，当计数为1
        close(connfd);
    }

    return 0;
}

