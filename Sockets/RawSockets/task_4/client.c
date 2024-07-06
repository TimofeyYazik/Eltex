#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
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
#define IP_ADDRESS "127.0.0.1"
#define SIZE_BUFF 114
#define SA struct sockaddr
#define CHL_LEVEL 14
#define NAME_PC "enp0s3"
#define TYPE_IPV4 0x0800
#define handler_error(text) \
do{ perror(text); exit(EXIT_FAILURE); } while(1);

typedef struct {
    uint8_t dest_mac[6];
    uint8_t source_mac[6];
    uint16_t type;
} ethernet_frame_t;

void CalcCheckSum(struct iphdr *ip_header) {
    ip_header->check = 0;
    uint32_t check = 0;
    uint16_t *counter = (uint16_t *)ip_header;
    for (int i = 0; i < sizeof(struct iphdr) / 2; i++) {
        check += ntohs(counter[i]);
    }
    while (check >> 16) {
        check = (check & 0xFFFF) + (check >> 16);
    }
    ip_header->check = ~check;
}

int main() {
    char buff_send[SIZE_BUFF] = {0};
    char buff_recv[SIZE_BUFF] = {0};
    struct sockaddr_ll server_endpoint, client_point;
    memset(&server_endpoint, 0, sizeof(server_endpoint));
    server_endpoint.sll_family = AF_PACKET;
    server_endpoint.sll_ifindex = if_nametoindex(NAME_PC);
    server_endpoint.sll_halen = 6;
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

    ethernet_frame_t *frame = (ethernet_frame_t *)buff_send;
    frame->source_mac[0] = 0x08;
    frame->source_mac[1] = 0x00;
    frame->source_mac[2] = 0x27;
    frame->source_mac[3] = 0x68;
    frame->source_mac[4] = 0xb9;
    frame->source_mac[5] = 0xf2;

    frame->dest_mac[0] = 0xc4;
    frame->dest_mac[1] = 0xb3;
    frame->dest_mac[2] = 0x01;
    frame->dest_mac[3] = 0xd7;
    frame->dest_mac[4] = 0xe8;
    frame->dest_mac[5] = 0x6d;
    frame->type = htons(TYPE_IPV4); 

    struct iphdr *ihdr = (struct iphdr *)(buff_send + CHL_LEVEL);
    ihdr->version = 4;
    ihdr->ihl = 5;
    ihdr->tos = 0;
    ihdr->tot_len = htons(SIZE_BUFF - CHL_LEVEL);
    ihdr->id = htons(12134);
    ihdr->frag_off = 0;
    ihdr->ttl = 255;
    ihdr->protocol = IPPROTO_UDP;
    inet_pton(AF_INET, IP_ADDRESS, &ihdr->saddr);
    inet_pton(AF_INET, IP_ADDRESS, &ihdr->daddr);
    CalcCheckSum(ihdr);

    struct udphdr *udph = (struct udphdr *)(buff_send + sizeof(struct iphdr) + CHL_LEVEL);
    udph->source = htons(SOURCE_PORT);
    udph->dest = htons(PORT);
    udph->len = htons(SIZE_BUFF - sizeof(struct iphdr) - CHL_LEVEL);
    udph->check = 0;

    int size = sizeof(client_point);
    char *data = buff_send + sizeof(struct udphdr) + sizeof(struct iphdr) + CHL_LEVEL;

    while (1) {
        printf("Enter message (to exit enter 'exit'):\n");
        scanf("%79s", data);
        if (strcmp(data, "exit") == 0) break;
        if (sendto(cfd, buff_send, SIZE_BUFF, 0, (SA*)&server_endpoint, sizeof(server_endpoint)) == -1) {
            handler_error("sendto");
        }
        printf("Message sent!\n");

        while (1) {
            recvfrom(cfd, buff_recv, SIZE_BUFF, 0, (SA*)&client_point, &size);
            udph = (struct udphdr *)(buff_recv + sizeof(struct iphdr) + CHL_LEVEL);
            if (udph->dest == htons(SOURCE_PORT)) {
                printf("%s\n", buff_recv + sizeof(struct iphdr) + sizeof(struct udphdr) + CHL_LEVEL);
                break;
            }
        }
    }

    close(cfd);
    return 0;
}
