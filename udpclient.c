#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sock;
    struct sockaddr_in server_addr;
    socklen_t len = sizeof(server_addr);
    char buffer[256];

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // No connect() needed in UDP
    printf("UDP Client ready. Type messages.\n\n");

    while (1) {
        // Send to server
        printf("You: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        // sendto() — must give server address every single time
        // UDP has no connection so it doesn't remember the destination
        sendto(sock, buffer, strlen(buffer), 0,
               (struct sockaddr *)&server_addr, len);
        if (strcmp(buffer, "exit") == 0) break;

        // Receive from server
        memset(buffer, 0, sizeof(buffer));
        recvfrom(sock, buffer, sizeof(buffer), 0,
                 (struct sockaddr *)&server_addr, &len);
        printf("Server: %s\n", buffer);
        if (strcmp(buffer, "exit") == 0) break;
    }

    printf("Chat ended.\n");
    close(sock);
    return 0;
}