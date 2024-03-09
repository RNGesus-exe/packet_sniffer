#include "sniffer.h"
#include <netinet/ip.h>

void processPacket(unsigned char* buffer, int size, int* tcp_pkts) {

    struct iphdr* iph = (struct iphdr*)buffer;

    // TODO: other packets + implement case for other protocols
    switch (iph->protocol) {
    case 6:
        ++tcp_pkts;
        printTcpPacket(buffer, size);
        break;
    }
}

void printTcpPacket(unsigned char* buffer, int size) {}
