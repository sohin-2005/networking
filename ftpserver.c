#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>

int main()
{
    int server_sock, client_sock;

    struct sockaddr_in server_addr, client_addr;

    socklen_t len = sizeof(client_addr);

    char buffer[1024];
    char filename[1024];

    // Create socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);

    // Setup address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    // Bind and listen
    bind(server_sock,
         (struct sockaddr *)&server_addr,
         sizeof(server_addr));

    listen(server_sock, 5);

    printf("Waiting for connection...\n");

    // Accept connection
    client_sock = accept(server_sock,
                        (struct sockaddr *)&client_addr,
                        &len);

    printf("Client connected\n");

    // Receive filename
    memset(filename, 0, sizeof(filename));

    read(client_sock, filename, sizeof(filename));

    printf("Requested filename: %s\n", filename);

    // Open file
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        strcpy(buffer, "File not found");

        send(client_sock, buffer, strlen(buffer), 0);
    }

    else
    {
        // Send file contents
        while (fgets(buffer, sizeof(buffer), fp) != NULL)
        {
            send(client_sock, buffer, strlen(buffer), 0);

            memset(buffer, 0, sizeof(buffer));
        }

        fclose(fp);

        printf("File sent successfully\n");
    }

    close(client_sock);
    close(server_sock);

    return 0;
}