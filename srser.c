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
    char ack[20];

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

    printf("Receiver waiting...\n\n");

    while (1) {

        memset(buffer, 0, sizeof(buffer));

        // Receive frame
        recvfrom(sockfd,
                 buffer,
                 sizeof(buffer),
                 0,
                 (struct sockaddr *)&client_addr,
                 &len);

        printf("Received Frame: %s\n", buffer);

        int random = rand() % 100;

        // Simulate ACK loss
        if (random < 70) {

            sprintf(ack, "ACK %s", buffer);

            sendto(sockfd,
                   ack,
                   strlen(ack),
                   0,
                   (struct sockaddr *)&client_addr,
                   len);

            printf("%s sent\n\n", ack);

        } else {

            printf("ACK lost for Frame %s\n\n", buffer);
        }
    }

    close(sockfd);

    return 0;
}
