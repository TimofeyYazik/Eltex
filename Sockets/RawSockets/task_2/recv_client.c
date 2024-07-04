#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#define PORT 6666
#define SOURCE_PORT 7777
#define ADDRES_IP "127.0.0.2"
#define IP_ADDRES "127.0.0.1"
#define SIZE_BUFF 80
#define SA struct sockaddr
#define handler_error(text) \
do{ perror(text); exit(EXIT_FAILURE); } while(1);


int main() {
    int cfd = 0;
    char buff_send[SIZE_BUFF] = {0};
    char buff_recv[SIZE_BUFF] = {0};
    struct sockaddr_in server_endpoint, client_settings;
    memset(&server_endpoint, 0, sizeof(server_endpoint));
    memset(&client_settings, 0, sizeof(client_settings));
    socklen_t size = sizeof(client_settings);

    client_settings.sin_port = htons(SOURCE_PORT);
    client_settings.sin_family = AF_INET;
    inet_pton(AF_INET,ADDRES_IP,&client_settings.sin_addr);
    server_endpoint.sin_family = AF_INET;
    server_endpoint.sin_port = htons(PORT);
    inet_pton(AF_INET, IP_ADDRES, &server_endpoint.sin_addr);

    cfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (cfd == -1) {
        handler_error("socket");
    }
    bind(cfd, &client_settings, sizeof(client_settings));
  
    while (1) {
        printf("enter messege (to exit enter 'exit'):\n");
        scanf("%9s", buff_send);
        if (strcmp(buff_send, "exit") == 0) break;
        if (sendto(cfd, buff_send, SIZE_BUFF, 0, (SA*)&server_endpoint, sizeof(server_endpoint)) == -1) {
            handler_error("sendto");
        }
        printf("messege send!\n");
        while (1) {
            printf("LOOP\n");
            recvfrom(cfd, buff_recv, SIZE_BUFF, 0, (SA*)&client_settings, &size);
            printf("client recv\n");
            printf("%s\n", buff_recv);
        }
    }

    close(cfd);
    return 0;
}
