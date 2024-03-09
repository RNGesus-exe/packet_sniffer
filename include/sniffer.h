#ifndef SNIFFER_H
#define SNIFFER_H

#include "globals.h"

void processPacket(unsigned char*, int, int*);
void printIpHeader(unsigned char*, int);
void printTcpPacket(unsigned char*, int);
void logCSVTcpPacket(struct iphdr*, struct tcphdr*);
void logPacketFeatures(struct iphdr*, struct tcphdr*);
void printUdpPacket(unsigned char*, int);
void printIcmpPacket(unsigned char*, int);
void printRawData(unsigned char*, int);

#endif // SNIFFER_H
