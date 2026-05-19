#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // Setup server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect
    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Connected! You go first.\n\n");

    while (1) {
        // Send to server
        printf("You: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        send(sock, buffer, strlen(buffer), 0);
        if (strcmp(buffer, "exit") == 0) break;

        // Receive from server
        memset(buffer, 0, sizeof(buffer));
        read(sock, buffer, sizeof(buffer));
        printf("Server: %s\n", buffer);
        if (strcmp(buffer, "exit") == 0) break;
    }

    printf("Chat ended.\n");
    close(sock);
    return 0;
}