#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len = sizeof(client_addr);
    char buffer[256];

    // SOCK_DGRAM = UDP (datagram, connectionless)
    // SOCK_STREAM = TCP (stream, connection-based)
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    // bind() is still needed — server must attach to a port
    bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // No listen(), No accept() — UDP has no connection setup
    printf("UDP Server ready on port 8080...\n\n");

    while (1) {
        // recvfrom() — like read() but also tells you WHO sent it
        // client_addr gets filled with the sender's IP and port
        memset(buffer, 0, sizeof(buffer));
        recvfrom(sock, buffer, sizeof(buffer), 0,
                 (struct sockaddr *)&client_addr, &len);
        printf("Client: %s\n", buffer);
        if (strcmp(buffer, "exit") == 0) break;

        // sendto() — like send() but you must specify WHERE to send
        // we send back to client_addr (the address recvfrom gave us)
        printf("You: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        sendto(sock, buffer, strlen(buffer), 0,
               (struct sockaddr *)&client_addr, len);
        if (strcmp(buffer, "exit") == 0) break;
    }

    printf("Chat ended.\n");
    close(sock);
    return 0;
}