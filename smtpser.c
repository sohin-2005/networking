//server

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>

#define PORT 2525
#define BUFFER_SIZE 1024


void handle_msg(int clientsock){
    char buffer[BUFFER_SIZE]={0};
    char msg[BUFFER_SIZE]={0};

    strcpy(msg,"220 SMTP Server Ready\r\n");

    send(clientsock,msg,strlen(msg),0);

    int datamode=0;

    while(1){
        memset(buffer,0,BUFFER_SIZE);

        int bytes_received=recv(clientsock,buffer,BUFFER_SIZE-1,0);

        if(bytes_received <= 0){
            printf("Client Disconnected\n");
            break;
        }
        printf("%s",buffer);

        if(datamode){

            if(strstr(buffer,".\r\n") != NULL){
                send(clientsock,"250 MESSAGE ACCEPTED\r\n",23,0);
                printf("Mail Received Successfully\n");

                datamode=0;
            }

            else{
                printf("Mail Data : %s",buffer);
            }

            continue;

        }


        if(strncmp(buffer,"HELO",4) == 0){
            
            send(clientsock,"250 HELLO\r\n",11,0);

        }else if( strncmp(buffer,"MAIL FROM",9) == 0){
            send(clientsock,"250 OK\r\n",8,0);


        }else if( strncmp(buffer,"RCPT TO",7) == 0){
            send(clientsock,"250 OK\r\n",8,0);


        }else if(strncmp(buffer,"DATA",4) == 0){

            strcpy(msg,"354 END DATA WITH <CR><LF>.<CR><LF>\r\n");
            send(clientsock,msg,strlen(msg),0);
            datamode=1;


        }else if(strncmp(buffer,"QUIT",4) == 0){
            send(clientsock,"221 BYE\r\n",9,0);
            break;

        }else{
            strcpy(msg,"500 UNKNOWN COMMAND\r\n");
            send(clientsock,msg,strlen(msg),0);
        }
    }

    close(clientsock);


}

int main(){
    int sockfd,newsock;
    struct sockaddr_in server,client;
    socklen_t len=sizeof(client);

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    
    if(sockfd < 0){
        perror("Socket Creation Failed");
        exit(EXIT_FAILURE);
    }
    server.sin_family=AF_INET;
    server.sin_port=htons(PORT);
    server.sin_addr.s_addr=INADDR_ANY;

    if( bind(sockfd,(struct sockaddr *)&server , sizeof(server)) < 0 ){
        perror("Bind Failed");
        exit(EXIT_FAILURE);
    }

    if(listen(sockfd,3) < 0){
        perror("listen Failed");
        exit(EXIT_FAILURE);
    }

    printf("SMTP server running on port %d\n",PORT);

    while(1){
        newsock=accept(sockfd,(struct sockaddr *)&client,&len);

        if(newsock < 0){
            perror("Accept Failed");
            exit(EXIT_FAILURE);
        }


        handle_msg(newsock);

        return 0 ;
    }


}

