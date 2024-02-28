#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/ip_icmp.h>
#include<netinet/udp.h>
#include<netinet/tcp.h>
#include<netinet/ip.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

FILE *arrayfile;

void ProcessPacket(unsigned char*, int);
void LogPacketFeatures(struct iphdr*, struct tcphdr*);

int main() {
    int saddr_size, data_size, sock_raw;
    struct sockaddr saddr;
    unsigned char *buffer = (unsigned char *)malloc(65536); // Buffer to store the packet

    arrayfile = fopen("arrays.txt", "w");
    if(arrayfile == NULL) {
        printf("Error creating arrays.txt file.\n");
        return 1;
    }

    sock_raw = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if(sock_raw < 0) {
        printf("Socket Error\n");
        return 1;
    }

    while(1) {
        saddr_size = sizeof saddr;
        // Receive a packet
        data_size = recvfrom(sock_raw, buffer, 65536, 0, &saddr, &saddr_size);
        if(data_size <0) {
            printf("Recvfrom error, failed to get packets\n");
            break;
        }
        ProcessPacket(buffer, data_size);
    }
    close(sock_raw);
    fclose(arrayfile);
    return 0;
}

void ProcessPacket(unsigned char* buffer, int size) {
    struct iphdr *iph = (struct iphdr*)buffer;
    if(iph->protocol == 6) { // Check if it is a TCP packet
        struct tcphdr *tcph=(struct tcphdr*)(buffer + iph->ihl*4);
        LogPacketFeatures(iph, tcph);
    }
}

void LogPacketFeatures(struct iphdr *iph, struct tcphdr *tcph) {
    // Example feature array [Source IP, Destination IP, Source Port, Destination Port, ...]
    // Convert IP addresses from int to dotted notation
    char sourceIp[INET_ADDRSTRLEN], destIp[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(iph->saddr), sourceIp, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(iph->daddr), destIp, INET_ADDRSTRLEN);

    // Log the extracted features and placeholders for non-extractable features
    fprintf(arrayfile, "[%s, %s, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, 0, 0, 0, ...],\n",
            sourceIp, destIp,
            ntohs(tcph->source), ntohs(tcph->dest),
            ntohl(tcph->seq), ntohl(tcph->ack_seq),
            (unsigned int)tcph->doff,
            (unsigned int)tcph->urg, (unsigned int)tcph->ack,
            (unsigned int)tcph->psh, (unsigned int)tcph->rst,
            (unsigned int)tcph->syn, (unsigned int)tcph->fin,
            ntohs(tcph->window));
    // Note: The zeros and ellipsis at the end represent placeholders for non-extractable or unimplemented features.
}

