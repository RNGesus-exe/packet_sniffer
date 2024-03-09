#ifndef GLOBALS_H
#define GLOBALS_H

#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// MEMORY SIZES
#define BUFFER_SIZE 65536

// ERROR CODES
#define MEMORY_ALLOC_ERR -1
#define FILE_OPEN_ERR -2
#define SOCK_ERR -3
#define RECVFROM_ERR -4

#endif // GLOABLS_H
