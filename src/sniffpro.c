#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/ip_icmp.h> // Not used in this example, but included for completeness
#include<netinet/udp.h>     // Not used in this example, but included for completeness
#include<netinet/tcp.h>
#include<netinet/ip.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

FILE *logfile;
FILE *csvfile;

void ProcessPacket(unsigned char*, int);
void print_ip_header(unsigned char*, int);
void print_tcp_packet(unsigned char*, int);
void LogCsvTcpPacket(struct iphdr*, struct tcphdr*);

int main() {
    int saddr_size, data_size, sock_raw;
    struct sockaddr saddr;
    unsigned char *buffer = (unsigned char *)malloc(65536); // Allocate memory for packet capture

    logfile = fopen("log.txt", "w");
    csvfile = fopen("data.csv", "w");
    if(logfile == NULL || csvfile == NULL) {
        printf("Error creating files.\n");
        return 1;
    }

    // Write the CSV header
    fprintf(csvfile, "Source IP,Destination IP,Source Port,Destination Port,Sequence Number,Acknowledgment Number,Header Length,URG,ACK,PSH,RST,SYN,FIN,Window Size\n");

    sock_raw = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if(sock_raw < 0) {
        printf("Socket Error\n");
        return 1;
    }

    while(1) {
        saddr_size = sizeof saddr;
        data_size = recvfrom(sock_raw, buffer, 65536, 0, &saddr, &saddr_size);
        if(data_size < 0) {
            printf("Recvfrom error, failed to get packets\n");
            break;
        }
        ProcessPacket(buffer, data_size);
    }
    close(sock_raw);
    printf("Finished\n");
    fclose(logfile);
    fclose(csvfile);
    return 0;
}

void ProcessPacket(unsigned char* buffer, int size) {
    struct iphdr *iph = (struct iphdr*)buffer;
    if(iph->protocol == 6) { // TCP Protocol
        print_tcp_packet(buffer, size);
    }
}

void print_ip_header(unsigned char* Buffer, int Size) {
    struct iphdr *iph = (struct iphdr*)Buffer;
    struct sockaddr_in source, dest;
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;
    // IP Header logging can be placed here if needed for the logfile
}

void print_tcp_packet(unsigned char* Buffer, int Size) {
    struct iphdr *iph = (struct iphdr *)Buffer;
    int iphdrlen = iph->ihl*4;
    struct tcphdr *tcph = (struct tcphdr*)(Buffer + iphdrlen);
    print_ip_header(Buffer, Size); // Optional: Log IP header details to logfile
    LogCsvTcpPacket(iph, tcph); // Log TCP packet details to CSV
}

void LogCsvTcpPacket(struct iphdr *iph, struct tcphdr *tcph) {
    char sourceIp[INET_ADDRSTRLEN], destIp[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(iph->saddr), sourceIp, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(iph->daddr), destIp, INET_ADDRSTRLEN);

    fprintf(csvfile, "%s,%s,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u\n",
            sourceIp,
            destIp,
            ntohs(tcph->source),
            ntohs(tcph->dest),
            ntohl(tcph->seq),
            ntohl(tcph->ack_seq),
            (unsigned int)tcph->doff,
            (unsigned int)tcph->urg,
            (unsigned int)tcph->ack,
            (unsigned int)tcph->psh,
            (unsigned int)tcph->rst,
            (unsigned int)tcph->syn,
            (unsigned int)tcph->fin,
            ntohs(tcph->window));
}

