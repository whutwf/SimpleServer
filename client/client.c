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
    char sendline[MAXLINE], recvline[MAXLINE];
    while(fgets(sendline, MAXLINE, fp) != NULL) {

        if (write(sockfd, sendline, strlen(sendline)) < 0 ) {
            perror("[client] Error: write sockfd\n");
        }
        if (read(sockfd, recvline, MAXLINE) == 0) {
            perror("[client] Error；server terminated prematurely\n");
        }

        fputs(recvline, stdout);
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