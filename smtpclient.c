//client
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>

#define PORT 2525
#define BUFFER_SIZE 1024

void s_and_r(int server_sock,const char *msg){

    char buffer[BUFFER_SIZE]={0};

    send(server_sock,msg,strlen(msg),0);
    printf("C: %s",msg);

    memset(buffer,0,BUFFER_SIZE);

    int n= recv(server_sock,buffer,BUFFER_SIZE,0);
    if(n > 0){
        printf("S: %s",buffer);
    }
}


int main(){
    int sockfd;
    struct sockaddr_in server;
    char buffer[BUFFER_SIZE]={0};
    
    sockfd=socket(AF_INET,SOCK_STREAM,0);

    if(sockfd < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server.sin_family=AF_INET;
    server.sin_port=htons(PORT);
    server.sin_addr.s_addr=inet_addr("127.0.0.1");

    if( connect(sockfd,(struct sockaddr *)&server,sizeof(server)) < 0){
        perror("connection failed");
        exit(EXIT_FAILURE);
    }

    memset(buffer,0,BUFFER_SIZE);

    recv(sockfd,buffer,BUFFER_SIZE-1,0);

   printf("S: %s",buffer);

   s_and_r(sockfd,"HELO localhost\r\n");
   s_and_r(sockfd,"MAIL FROM:abc@example.com\r\n");
   s_and_r(sockfd,"RCPT TO: xyz@mail.com\r\n");
   s_and_r(sockfd,"DATA\r\n");

   char *msg="SUBJECT: TEST MAIL\r\n";
   send(sockfd,msg,strlen(msg),0);

   char str[1024];

   printf("Enter the mail body:\n");

   fgets(str,BUFFER_SIZE,stdin);

   send(sockfd,str,strlen(str),0);

   msg=".\r\n";
   send(sockfd,msg,strlen(msg),0);

   printf("C:<message body sent>\n");

   recv(sockfd,buffer,BUFFER_SIZE,0);
   printf("S: %s",buffer);

   s_and_r(sockfd,"QUIT\r\n");

   close(sockfd);


   return 0;
}
