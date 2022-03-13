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

    int sock_fd_rcv, sock_fd_snd;
    struct sockaddr_in serv_addr, cli_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    memset(&cli_addr, 0, sizeof(cli_addr));

    sock_fd_rcv = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock_fd_rcv < 0){
        perror("socket creation failed");
        exit(1);
    }

    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(27312);

    int a = 1;
    setsockopt(sock_fd_rcv, SOL_SOCKET, SO_BROADCAST, &a, sizeof(a));
    bind(sock_fd_rcv, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
    
    while (1) {
        socklen_t len = sizeof(cli_addr);
        recvfrom(sock_fd_rcv, buf, sizeof(buf), MSG_WAITALL,
                 (struct sockaddr *) &cli_addr, &len);

        printf("Got message from broadcast, ip = %s, port = %d\nMessage : %s",
               inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), buf);

        sock_fd_snd = socket(AF_INET, SOCK_DGRAM, 0);
        char buf_answer[1024] = "I am Bagr server";
        sendto(sock_fd_snd, buf_answer, strlen(buf_answer), MSG_CONFIRM, (const struct sockaddr *) &cli_addr, sizeof cli_addr);
        memset(buf_answer, 0, sizeof buf_answer);
        memset(buf, 0, sizeof buf);
    }
    close(sock_fd_rcv);
}