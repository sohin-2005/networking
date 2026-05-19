#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <time.h>

#define PORT 8080
#define SIZE 1024

int main() {

    int sockfd;

    struct sockaddr_in server_addr, client_addr;

    socklen_t len = sizeof(client_addr);

    char buffer[SIZE];

    int ack_prob = 70;

    srand(time(0));

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Setup address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind
    bind(sockfd,
         (struct sockaddr *)&server_addr,
         sizeof(server_addr));

    printf("Waiting for packets...\n\n");

    while (1) {

        memset(buffer, 0, sizeof(buffer));

        // Receive packet
        int n = recvfrom(sockfd,
                         buffer,
                         sizeof(buffer),
                         0,
                         (struct sockaddr *)&client_addr,
                         &len);

        if (n <= 0)
            break;

        printf("Received packet: %s\n", buffer);

        // Random ACK simulation
        if (rand() % 100 < ack_prob) {

            printf("ACK sent for packet %s\n\n", buffer);

            sendto(sockfd,
                   "ACK",
                   strlen("ACK"),
                   0,
                   (struct sockaddr *)&client_addr,
                   len);

        } else {

            printf("ACK lost for packet %s\n\n", buffer);
        }
    }

    close(sockfd);

    return 0;
}