#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAXLINE 1024
#define PORT 1e4

int main(int argc, char** argv) {
    if(argc != 2){
        printf("Wrong format\n");
        exit(1);
    }

    char buf[1024] = {0};
    if(!strcmp(argv[1], "TCP")){
        printf("TCP mode\n");

        int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
        if(sock_fd == -1){
            printf("sock_fd == -1\n");
            exit(1);
        }
        struct sockaddr_in serv_addr;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(1e4);
        serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

        int connect_fd = connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        printf("connect_fd == %d\n", connect_fd);
        if(connect_fd < 0){
            printf("connect_fd = %d\n", connect_fd);
            exit(1);
        }

        int n;
        while (1){
            n = read(STDIN_FILENO, buf, 256);
            write(sock_fd, buf, n);
        }
        printf("n = %d\n", n);
    }
    else{ // UDP
        int sock_fd;
        char buf[MAXLINE] = {0};
        struct sockaddr_in serv_addr;

        // Creating socket file descriptor
        if ( (sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }

        memset(&serv_addr, 0, sizeof(serv_addr));

        // Filling server information
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);
        serv_addr.sin_addr.s_addr = INADDR_ANY;

        int n;
        while (1){
            n = read(STDIN_FILENO, buf, MAXLINE);
            printf("buf = %s", buf);
            if(!strcmp(buf, "-quit\n")){
                break;
            }
            sendto(sock_fd, buf, n, MSG_CONFIRM, (const struct sockaddr *) &serv_addr, sizeof serv_addr);
            memset(buf, 0, sizeof(buf));
        }

        close(sock_fd);
        return 0;
    }
    return 0;
}

