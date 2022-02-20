#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAXLINE 1024

int main(){
    char buf[MAXLINE] = {0};

    int sock_fd;
    struct sockaddr_in serv_addr, cli_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    memset(&cli_addr, 0, sizeof(cli_addr));

    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock_fd < 0){
        perror("socket creation failed");
        exit(1);
    }

    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(27312);

    int a;
    setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, &a, sizeof(a));
    bind(sock_fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));

    socklen_t len = sizeof(cli_addr);
    recvfrom(sock_fd, buf, sizeof (buf), MSG_WAITALL,
             (struct sockaddr *) &cli_addr, &len);

    printf("Got message from broadcast, ip = %s, port = %d\nMessage : %s",
           inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), buf);

    char buf_answer[1024] = "I get - ";
    strcat(buf_answer, buf);
    sendto(sock_fd, buf_answer, strlen(buf_answer), MSG_CONFIRM, (const struct sockaddr *) &cli_addr, sizeof cli_addr);

    close(sock_fd);
}