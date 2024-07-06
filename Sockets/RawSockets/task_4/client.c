//#ifdef __linux__
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <net/if.h>

#define PORT 6666
#define SOURCE_PORT 7777
#define SOURCE_IP_ADDRES "127.0.0.1"
#define DEST_IP_ADDRES "127.0.0.2"
#define SIZE_BUFF 114
#define SA struct sockaddr
#define CHL_LEVEL 14
#define NAME_PC "enp0s3"
#define TYPE_IPV4 0x0800
#define handler_error(text) \
do{ perror(text); exit(EXIT_FAILURE); } while(1);

typedef  struct{
  char dest_mac[6];
  char source_mac[6];
  int16_t type;    
} ethernet_frame_t;

void CalcCheckSum(struct iphdr *ip_header, int *check){
    int16_t *counter = (int16_t *)ip_header;
    for(int i = 0; i < 10; i++){
        (*check) += (*counter);        
        counter++;
    }
    while(1){
        int overflow = *check >> 16;
        if(!overflow) break;
        *check = (*check & 0xFFFF) + overflow;
    }
    *check = ~(*check); 
}

int main() {
    char buff_send[SIZE_BUFF] = {0};
    char buff_recv[SIZE_BUFF] = {0};
    struct sockaddr_ll server_endpoint, client_point;
    memset(&server_endpoint, 0, sizeof(server_endpoint));
    server_endpoint.sll_family = AF_PACKET;
    server_endpoint.sll_ifindex = if_nametoindex(NAME_PC);
    server_endpoint.sll_halen = 6;
    //c4:b3:01:d7:e8:6d
    server_endpoint.sll_addr[0] = 0xc4; 
    server_endpoint.sll_addr[1] = 0xb3;
    server_endpoint.sll_addr[2] = 0x01;
    server_endpoint.sll_addr[3] = 0xd7;
    server_endpoint.sll_addr[4] = 0xe8;
    server_endpoint.sll_addr[5] = 0x6d;
    int cfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (cfd == -1) {
        handler_error("socket");
    }

    ethernet_frame_t *frame = (ethernet_frame_t*)buff_send;
    //08:00:27:68:b9:f2
    frame->source_mac[0] = 0x08;
    frame->source_mac[1] = 0x00;
    frame->source_mac[2] = 0x27;
    frame->source_mac[3] = 0x68;
    frame->source_mac[4] = 0xb9;
    frame->source_mac[5] = 0xf2;
    //c4:b3:01:d7:e8:6d
    frame->dest_mac[0] = 0xc4;
    frame->dest_mac[1] = 0xb3;
    frame->dest_mac[2] = 0x01;
    frame->dest_mac[3] = 0xd7;
    frame->dest_mac[4] = 0xe8;
    frame->dest_mac[5] = 0x6d;
    frame->type = TYPE_IPV4;
    
    
    struct iphdr  *ihdr = (struct iphdr *)(buff_send + CHL_LEVEL);    
    ihdr->version = 4;
    ihdr->tos = 0;
    ihdr->ihl = 5;
    ihdr->tot_len = SIZE_BUFF;
    ihdr->id = htonl(12134);
    ihdr->frag_off = 0;
    ihdr->ttl= 255;
    ihdr->protocol= IPPROTO_UDP;
    ihdr->check = 0;
    inet_pton(AF_INET, SOURCE_IP_ADDRES, &ihdr->saddr);
    inet_pton(AF_INET, DEST_IP_ADDRES, &ihdr->daddr);
    CalcCheckSum(ihdr, &ihdr->check);
    
    struct udphdr *udph = (struct udphdr *)(buff_send + sizeof(struct iphdr) + CHL_LEVEL);
    udph->source = htons(SOURCE_PORT);
    udph->dest = htons(PORT);
    udph->len = htons(SIZE_BUFF - sizeof(struct iphdr));
    udph->check = 0;
    inr size = sizeof(client_point);
    char *data = buff_send + sizeof(struct udphdr) + sizeof(struct iphdr) + CHL_LEVEL;
    while (1) {
        printf("enter messege (to exit enter 'exit'):\n");
        scanf("%79s", data);
        if (strcmp(data, "exit") == 0) break;
        if (sendto(cfd, buff_send, SIZE_BUFF, 0, (SA*)&server_endpoint, sizeof(server_endpoint)) == -1) {
            handler_error("sendto");
        }
        printf("messege send!\n");
        while (1) {
            recvfrom(cfd, buff_recv, SIZE_BUFF, 0, (SA*)&client_point, &size);
            udph = (struct udphdr *)(buff_recv + sizeof(struct iphdr) + CHL_LEVEL);
            if(udph->dest == htons(SOURCE_PORT)){
               printf("%s\n", buff_recv + 36);
               break;
            }
        }
    }

    close(cfd);
    return 0;
}
//#endif
