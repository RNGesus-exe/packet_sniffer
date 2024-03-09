#include "sniffer.h"

int main(int argc, char* argv[]) {

    int sock_raw = 0;
    FILE* log_file = NULL;
    int tcp = 0;
    struct sockaddr_in source, dest;
    socklen_t saddr_size;
    int data_size;
    struct sockaddr saddr;
    struct in_addr inaddr;
    int total = 0; // Keeps track of total packets processed so far

    unsigned char* buffer = (unsigned char*)malloc(BUFFER_SIZE);
    if (!buffer) {
        fprintf(stderr, "SNx00: MEMORY COULT NOT BE ALLOCATED\n");
        return MEMORY_ALLOC_ERR;
    }

    log_file = fopen("log.txt", "wb");
    if (!log_file) {
        fprintf(stderr, "SNx01: LOGFILE COULD NOT BE OPENED\n");
        return FILE_OPEN_ERR;
    }

    printf("Starting...\n");

    sock_raw = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sock_raw < 0) {
        fprintf(stderr, "SNx02: SOCKET ERROR\n");
        return SOCK_ERR;
    }

    while (1) {
        saddr_size = sizeof(saddr);
        data_size = recvfrom(sock_raw, buffer, BUFFER_SIZE, 0, &saddr, &saddr_size);
        if (data_size < 0) {
            fprintf(stderr, "SNx03: RECVFROM ERROR, FAILED TO GET PACKETS\n");
            return RECVFROM_ERR;
        }

        processPacket(buffer, data_size, &total);
    }

    close(sock_raw);
    return 0;
}
