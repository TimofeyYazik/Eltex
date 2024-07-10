#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <net/if.h>

#define D_PORT 6666
#define S_PORT 7777
#define D_IP_ADDRES "192.168.56.2"
#define S_IP_ADDRES "192.168.56.3"
#define SIZE_BUFF 114
#define SA struct sockaddr
#define CHL_OFFSET 14
#define NAME_INTERFACE "enp0s8"
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
    for (int i = 0; i < 10; i++) {
        check += counter[i];
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
    server_endpoint.sll_ifindex = if_nametoindex(NAME_INTERFACE);
    server_endpoint.sll_halen = 6;
    server_endpoint.sll_addr[0] = 0x08;
    server_endpoint.sll_addr[1] = 0x00;
    server_endpoint.sll_addr[2] = 0x27;
    server_endpoint.sll_addr[3] = 0x78;
    server_endpoint.sll_addr[4] = 0x85;
    server_endpoint.sll_addr[5] = 0x25;
    int cfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (cfd == -1) {
        handler_error("socket");
    }

    ethernet_frame_t *frame = (ethernet_frame_t *)buff_send;
    frame->source_mac[0] = 0x08;
    frame->source_mac[1] = 0x00;
    frame->source_mac[2] = 0x27;
    frame->source_mac[3] = 0xd1;
    frame->source_mac[4] = 0xe9;
    frame->source_mac[5] = 0x19;

    frame->dest_mac[0] = 0x08;
    frame->dest_mac[1] = 0x00;
    frame->dest_mac[2] = 0x27;
    frame->dest_mac[3] = 0x78;
    frame->dest_mac[4] = 0x85;
    frame->dest_mac[5] = 0x25;
    frame->type = htons(ETH_P_IP); 

    struct iphdr *ihdr = (struct iphdr *)(buff_send + CHL_OFFSET);
    ihdr->version = 4;
    ihdr->ihl = 5;
    ihdr->tos = 0;
    ihdr->tot_len = htons(SIZE_BUFF - CHL_OFFSET);
    ihdr->id = htons(12134);
    ihdr->frag_off = 0;
    ihdr->ttl = 255;
    ihdr->protocol = IPPROTO_UDP;
    inet_pton(AF_INET, S_IP_ADDRES, &ihdr->saddr);
    inet_pton(AF_INET, D_IP_ADDRES, &ihdr->daddr);
    CalcCheckSum(ihdr);

    struct udphdr *udph = (struct udphdr *)(buff_send + sizeof(struct iphdr) + CHL_OFFSET);
    udph->source = htons(S_PORT);
    udph->dest = htons(D_PORT);
    udph->len = htons(SIZE_BUFF - sizeof(struct iphdr) - CHL_OFFSET);
    udph->check = 0;
    int size = sizeof(client_point);
    char *data = buff_send + sizeof(struct udphdr) + sizeof(struct iphdr) + CHL_OFFSET;

    while (1) {
        printf("Enter message (to exit enter 'exit'):\n");
        scanf("%79s", data);
        if (strcmp(data, "exit") == 0) break;
        if (sendto(cfd, buff_send, SIZE_BUFF, 0, (SA*)&server_endpoint, sizeof(server_endpoint)) == -1) {
            handler_error("sendto");
        }
        printf("Message send!\n");

        while (1) {
            recvfrom(cfd, buff_recv, SIZE_BUFF, 0, (SA*)&client_point, &size);
            udph = (struct udphdr *)(buff_recv + sizeof(struct iphdr) + CHL_OFFSET);
            if (udph->dest == htons(S_PORT)) {
                printf("%s\n", buff_recv + sizeof(struct iphdr) + sizeof(struct udphdr) + CHL_OFFSET);
                break;
            }
        }
    }

    close(cfd);
    return 0;
}
