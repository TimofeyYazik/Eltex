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
#define IP_ADDRES "127.0.0.1"
#define SIZE_BUFF 108
#define SIZE_BUFF_SEND 88
#define SA struct sockaddr
#define handler_error(text) \
do{ perror(text); exit(EXIT_FAILURE); } while(1);

unsigned short checksum(void *b, int len) {    
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;
    
    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char*)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

int main() {
    int cfd = 0;
    char buff_send[SIZE_BUFF_SEND] = {0};
    char buff_recv[SIZE_BUFF] = {0};
    struct sockaddr_in server_endpoint, client_settings;
    memset(&server_endpoint, 0, sizeof(server_endpoint));
    memset(&client_settings, 0, sizeof(client_settings));
    socklen_t size = sizeof(client_settings);
    
    server_endpoint.sin_family = AF_INET;
    server_endpoint.sin_port = htons(PORT);
    inet_pton(AF_INET, IP_ADDRES, &server_endpoint.sin_addr);

    cfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (cfd == -1) {
        handler_error("socket");
    }
      
    //struct iphdr *iph = (struct iphdr *)buff_send;
    //iph->ihl = 5;
    //iph->version = 4;
    //iph->tos = 0;
    //iph->tot_len = htons(SIZE_BUFF);
    //iph->id = htonl(54321);
    //iph->frag_off = 0;
    //iph->ttl = 255;
    //iph->protocol = IPPROTO_UDP;
    //iph->check = 0;
    //iph->saddr = inet_addr(IP_ADDRES);
    //iph->daddr = server_endpoint.sin_addr.s_addr;
    //iph->check = checksum(iph, sizeof(struct iphdr));
        
    struct udphdr *udph = (struct udphdr *)(buff_send);
    udph->source = htons(SOURCE_PORT);
    udph->dest = htons(PORT);
    udph->len = htons(SIZE_BUFF);
    udph->check = 0;

    char *data = buff_send + sizeof(struct udphdr);
    while (1) {
        printf("enter messege (to exit enter 'exit'):\n");
        scanf("%79s", data);
        if (strcmp(data, "exit") == 0) break;
        if (sendto(cfd, buff_send, SIZE_BUFF, 0, (SA*)&server_endpoint, sizeof(server_endpoint)) == -1) {
            handler_error("sendto");
        }
        printf("messege send!\n");
        while (1) {
            printf("LOOP\n");
            recvfrom(cfd, buff_recv, SIZE_BUFF, 0, (SA*)&client_settings, &size);
            printf("client recv\n");
            printf("%s\n", buff_recv + 28);
            udph = (struct udphdr *)(buff_recv + sizeof(struct iphdr));
            if(udph->dest == htons(SOURCE_PORT)){
               printf("%s\n", buff_recv + 28);
               break;
            }
        }
    }

    close(cfd);
    return 0;
}
