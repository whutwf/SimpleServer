#include "client.h"

int main(int argc, char **argv)
{
    if (argc != 2) {
        perror("[client] Error: need two params\n");
    }
    go_client_ipv4(SERV_PORT, argv[1]);
}

void str_cli(int sockfd, FILE *fp)
{
    int maxfdp1;
    fd_set rset;
    int filefd = -1;

    char sendline[MAXLINE], recvline[MAXLINE];

    int n;
    int stdineof = 0;

    FD_ZERO(&rset);
    filefd =fileno(fp);
    for ( ; ; ) {
        if (stdineof == 0) {
             FD_SET(filefd, &rset);
        }
        FD_SET(sockfd, &rset);
        maxfdp1 = ((filefd > sockfd) ? filefd : sockfd) + 1;
        if (select(maxfdp1, &rset, NULL, NULL, NULL) <= 0) {
            perror("[client] Error: select\n");
            return;
        }

        /*使用select实现I/O复用， 阻塞在某一个上面*/
        /*shutdown可以单独关闭读与写，双工道*/
        if (FD_ISSET(sockfd, &rset)) {
            if ((n = read(sockfd, recvline, MAXLINE)) == 0) {
                if (stdineof == 1) {
                    return;
                } else {
                    perror("[client] Error；server terminated prematurely\n");
                } 
            }

            write(filefd, recvline, n);

        } else if (FD_ISSET(filefd, &rset)) {

            if ((n = read(filefd, sendline, MAXLINE)) == 0) {
                stdineof = 1;
                shutdown(sockfd, SHUT_WR);
                FD_CLR(filefd, &rset);
                continue;
            }

            if (write(sockfd, sendline, n) < 0 ) {
                perror("[client] Error: write sockfd\n");
            }

        }
    }
}

int go_client_ipv4(int port, char *ip_name)
{
    int sockfd = -1;
    struct sockaddr_in servaddr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("[client] Error: socket begin\n");
        return -1;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip_name, &servaddr.sin_addr) != 1) {
        perror("[client] Error: ip to net addr \n");
        close(sockfd);
        return -1;
    }

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("[client] Error: socket connect\n");
        close(sockfd);
        return -1;
    }

    str_cli(sockfd, stdin);

    close(sockfd);

    return 0;
}