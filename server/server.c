#include "server.h"

int main(int argc, char **argv)
{
    go_server_ipv4(SERV_PORT);
}

void str_echo(int sockfd)
{
    ssize_t n;
    char buf[MAXLINE];

#define SERVER_SUM

#ifdef SERVER_SUM
    long arg1, arg2;
#endif

again:
    //当服务器关闭时候，返回0
    while ((n = read(sockfd, buf, MAXLINE)) > 0) {

    #ifdef SERVER_ONLY
        if (write(sockfd, buf, n) != n) {
            fprintf(stdout, "[server] Info：write to client not full\n");
        }
    #endif

    #ifdef SERVER_SUM
       if (sscanf(buf, "%ld%ld", &arg1, &arg2) == 2) {
           snprintf(buf, sizeof(buf), "%ld\n", arg1 + arg2);
       } else {
           snprintf(buf, sizeof(buf), "[server] Error: client input error\n");
       }

       n = strlen(buf);
       if (write(sockfd, buf, n) != n) {
            fprintf(stdout, "[server] Info：write to client not full\n");
        }
    #endif
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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    /* Connection refused reason*/
    servaddr.sin_port = htons(port);

    if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("[server] Error: socket bind\n");
        close(listenfd);
        return -1;
    }
    if (listen(listenfd, LISTENQ) < 0) {
        perror("[server] Error: socket listen\n");
        close(listenfd);
        return -1;
    }
    //这个在子进程创建之前调用，并且只调用一次
    //捕捉僵死进程
    //阻塞到有第一个进程终止，内核会使accept返回一个EINTR错误，父进程如果不处理这个错误，或直接终止
    wrap_signal(SIGCHLD, sig_chld);

    for ( ; ; ) {
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
        if (connfd < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                perror("[server] Error: socket accept\n");
                close(listenfd);
                return -1;
            }
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

Sigfunc *wrap_signal(int signo, Sigfunc *func)
{
    struct sigaction act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (signo == SIGALRM) {
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;
#endif
    } else {
#ifdef SA_RESTRART
        act.sa_flags |= SA_RESTART;
#endif
    }

    if (sigaction(signo, &act, &oact) < 0) {
        return (SIG_ERR);
    }

    //返回上一个动作
    return (oact.sa_handler);
}

void sig_chld(int signo)
{
    pid_t pid;
    int stat;

    /*不选择使用wait的原因在于:当有大量的子进程几乎同时结束，则会同时产生SIGCHLD，但是wait要阻塞信号，只能处理其中一个，导致僵死
      而，waitpid可以控制不阻塞信号
    */
    // pid = wait(&stat);
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
        fprintf(stdout, "[wait] Info: child %d terminated\n", pid);
    }

    return;
}
