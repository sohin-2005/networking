#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define PORT 8080
#define SIZE 1024
#define WINDOW_SIZE 4

int main() {

    int sockfd;

    struct sockaddr_in server_addr;

    socklen_t len = sizeof(server_addr);

    char buffer[SIZE];
    char frame[20];

    struct timeval tv;

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Setup server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Timeout
    tv.tv_sec = 3;
    tv.tv_usec = 0;

    setsockopt(sockfd,
               SOL_SOCKET,
               SO_RCVTIMEO,
               (const char *)&tv,
               sizeof(tv));

    printf("Sender started\n\n");

    int base = 1;
    int total_frames = 8;

    while (base <= total_frames) {

        // Send window frames
        for (int i = base;
             i < base + WINDOW_SIZE && i <= total_frames;
             i++) {

            sprintf(frame, "%d", i);

            printf("Sending Frame %d \n", i);

            sendto(sockfd,
                   frame,
                   strlen(frame),
                   0,
                   (struct sockaddr *)&server_addr,
                   len);
        }

        // Receive ACKs
        for (int i = base;
             i < base + WINDOW_SIZE && i <= total_frames;
             i++) {

            memset(buffer, 0, sizeof(buffer));

            int n = recvfrom(sockfd,
                             buffer,
                             sizeof(buffer),
                             0,
                             NULL,
                             NULL);

            if (n > 0) {

                printf("%s received\n", buffer);

            } else {

                printf("Timeout for Frame %d\n", i);

                sprintf(frame, "%d", i);

                printf("Retransmitting Frame %d\n", i);

                sendto(sockfd,
                       frame,
                       strlen(frame),
                       0,
                       (struct sockaddr *)&server_addr,
                       len);
            }
        }

        base += WINDOW_SIZE;
    }

    printf("\nAll frames sent successfully\n");

    close(sockfd);

    return 0;
}
