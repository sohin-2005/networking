#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define PORT 8080
#define SIZE 1024

int main() {

    int sockfd;

    struct sockaddr_in server_addr;

    socklen_t len = sizeof(server_addr);

    char buffer[SIZE];
    char packet[50];

    struct timeval tv;

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Setup server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("UDP Client Started\n\n");

    // Set timeout
    tv.tv_sec = 3;
    tv.tv_usec = 0;

    setsockopt(sockfd,
               SOL_SOCKET,
               SO_RCVTIMEO,
               (const char *)&tv,
               sizeof(tv));

    int packet_no = 1;

    while (packet_no <= 5) {

        sprintf(packet, "%d", packet_no);

        printf("Sending packet %d...\n", packet_no);

        // Send packet
        sendto(sockfd,
               packet,
               strlen(packet),
               0,
               (struct sockaddr *)&server_addr,
               len);

        memset(buffer, 0, sizeof(buffer));

        // Receive ACK
        int n = recvfrom(sockfd,
                         buffer,
                         sizeof(buffer),
                         0,
                         NULL,
                         NULL);

        if (n > 0 && strcmp(buffer, "ACK") == 0) {

            printf("ACK received for packet %d\n\n", packet_no);

            packet_no++;

        } else {

            printf("Timeout! Retransmitting packet %d\n\n", packet_no);
        }
    }

    printf("All packets sent successfully\n");

    close(sockfd);

    return 0;
}