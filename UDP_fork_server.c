#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include<signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define MAX_SEND_BUF 1600

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in serv_addr, client_address;
    int address_len = sizeof(serv_addr);
    int cli_len = sizeof(client_address);

    /* 
    *  Socket() call takes three arguments:
    *  The family of protocol used/address family 
    *  Type of socket 
    *  Protocol number or 0 for a given family and type 
    */

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    /* 
    *  Socket call will return a socket descriptor on success which is an integer 
    *  And it will return '-1' for error
    */
    if (sockfd == -1)
    {
        printf("Error calling Socket");
        exit(1);
    }

    /* Populating the sockaddr_in struct with the following values */
    /* Assigning the AF_INET (Internet Protocol v4) address family */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8000);

    if(bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)/* Returns 0 for success and -1 for failure */
    {
        printf("error binding");
        exit(1);
    }

    signal(SIGCHLD, SIG_IGN);
    while(1)
    {
        char msg[1000];
        long data_len;
            
        data_len = recvfrom(sockfd,(char*)msg,sizeof(msg),0,(struct sockaddr *)&client_address, (socklen_t*)&cli_len);                             //recieve the file name from client
            
        if(data_len)
        {
            printf("\n\nClient connected to Multiprocess connectionless server uising fork call\n");
            printf("File name recieved: %s\n", msg); 
        }
        if (fork() == 0)
        {
            int file;                                                                   //read the local file (server file)
            if((file = open(msg,O_RDWR)) == -1)
            {
                char* invalid = "INVALID";
                printf("File not found\n");
                printf("Client disconnected\n\n\n");
                printf("%s\n",strerror(errno));
                sendto(sockfd, invalid, MAX_SEND_BUF, 0, (struct sockaddr*)&client_address, sizeof(client_address));
            }
            else
            {
                printf("File opened successfully\n");
                ssize_t read_bytes;
                ssize_t sent_bytes;
                
                char send_buf[MAX_SEND_BUF];
                while( (read_bytes = read(file, send_buf, MAX_SEND_BUF)) > 0 )          //read the contents of file on server
                {   
                }
                sent_bytes = sendto(sockfd,send_buf, 1600 , 0,(struct sockaddr *)&client_address, sizeof(client_address));

                printf("\n");
                close(file);
                printf("\nClient disconnected\n\n\n");
            }
        }
    }
}
