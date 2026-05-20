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

    int expected = 1;

    srand(time(0));

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0) {
        printf("Socket creation failed\n");
        return 1;
    }

    // Setup address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind
    if (bind(sockfd,
             (struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0) {

        printf("Bind failed\n");
        return 1;
    }

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

        int frame = atoi(buffer);

        printf("Received Frame %d\n", frame);

        int random = rand() % 100;

        // Correct frame received
        if (frame == expected && random < 70) {

            sprintf(ack, "%d", expected + 1);

            sendto(sockfd,
                   ack,
                   strlen(ack),
                   0,
                   (struct sockaddr *)&client_addr,
                   len);

            printf("ACK %d sent\n\n", expected + 1);

            expected++;

        } else {

            // Resend previous ACK
            sprintf(ack, "%d", expected);

            sendto(sockfd,
                   ack,
                   strlen(ack),
                   0,
                   (struct sockaddr *)&client_addr,
                   len);

            printf("Frame %d discarded\n", frame);
            printf("ACK %d resent\n\n", expected);
        }
    }

    close(sockfd);

    return 0;
}
