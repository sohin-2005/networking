#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len = sizeof(client_addr);
    char buffer[1024];

    // Create socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);

    // Setup address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    // Bind, listen, accept
    bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_sock, 5);
    printf("Waiting for client...\n");
    client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &len);
    printf("Client connected!\n\n");

    while (1) {
        // Receive from client
        memset(buffer, 0, sizeof(buffer));
        read(client_sock, buffer, sizeof(buffer));
        printf("Client: %s\n", buffer);
        if (strcmp(buffer, "exit") == 0) break;

        // Send to client
        printf("You: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;  // remove newline
        send(client_sock, buffer, strlen(buffer), 0);
        if (strcmp(buffer, "exit") == 0) break;
    }

    printf("Chat ended.\n");
    close(client_sock);
    close(server_sock);
    return 0;
}