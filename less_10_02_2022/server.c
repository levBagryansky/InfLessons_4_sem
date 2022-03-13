#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAXLINE 1024

int main(int argc, char** argv) {
    char buf[1024] = {0};
    if(argc != 2){
        printf("Wrong format\n");
        exit(1);
    }

    if(!strcmp(argv[1], "TCP")) { // TCP
        printf("TCP mode\n");

        struct sockaddr_in serv_addr;

        int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (sock_fd == -1) {
            printf("sock_fd == -1, exit\n");
            exit(1);
        }
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        serv_addr.sin_port = htons(1e4);

        bind(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
        printf("binded\n");
        listen(sock_fd, 2);
        printf("listened\n");
        int acc_fd = accept(sock_fd, NULL, NULL);
        printf("accepted\n");
        int n;
        while (1) {
            n = read(acc_fd, buf, MAXLINE);
            write(STDOUT_FILENO, buf, n);
        }
    }
    else{ // UDP
        printf("UDP mode\n");
        int sock_fd;
        struct sockaddr_in serv_addr, cli_addr;
        sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

        if (sock_fd < 0) {
            printf("sock_fd = %d", sock_fd);
        }

        memset(&serv_addr, 0, sizeof(serv_addr));
        memset(&cli_addr, 0, sizeof(cli_addr));

        serv_addr.sin_family    = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(1e4);

        if ( bind(sock_fd, (const struct sockaddr *)&serv_addr,
                  sizeof(serv_addr)) < 0 )
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }

        while (1) {
            int len = sizeof(cli_addr);  //len is value/resuslt

            int n = recvfrom(sock_fd, (char *) buf, MAXLINE,
                             MSG_WAITALL, (struct sockaddr *) &cli_addr, (socklen_t *) &len);

            buf[n] = '\0';
            write(STDOUT_FILENO, buf, n);
        }
    }
    return 0;
}
