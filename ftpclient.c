#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

int main()
{
    int sock;

    struct sockaddr_in server_addr;

    char filename[1024];
    char buffer[1024];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // Setup server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect
    connect(sock,
           (struct sockaddr *)&server_addr,
           sizeof(server_addr));

    printf("Connected to the server\n");

    // Get filename
    printf("Enter the filename: ");

    scanf("%s", filename);

    // Send filename
    send(sock, filename, strlen(filename), 0);

    printf("\nFile contents:\n\n");

    // Receive file contents
    while (read(sock, buffer, sizeof(buffer)) > 0)
    {
        printf("%s", buffer);

        memset(buffer, 0, sizeof(buffer));
    }

    close(sock);

    return 0;
}