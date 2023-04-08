

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    int sockfd;
    struct sockaddr_in serv_addr;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Unable to create socket\n");
        exit(0);
    }
    else 
    {
        printf("******* Socket created ******* \n") ;
    }
    serv_addr.sin_family = AF_INET;
    inet_aton("127.0.0.1", &serv_addr.sin_addr);
    serv_addr.sin_port = htons(20000);
    // printf("****** Socket created *****\n\n Waiting for client .....\n");
    if ((connect(sockfd, (struct sockaddr *)&serv_addr,
                 sizeof(serv_addr))) < 0)
    {
        perror("Unable to connect to server\n");
        exit(0);
    }
    else{
        printf("Successfully connected to server \n") ;
    }
    while (1)
    {
        char buff[20] ;
        int i = 0 , exit_flag = 0 ;
        printf("Enter the expression : ");
        char ch ;
        ch = getchar() ;
        while(1)
        {
            if( i == 20 )
            {
                send(sockfd, buff, 21 , 0);
                exit_flag = 1 ; 
                i = 0 ;
                for( int j = 0 ; j < 20 ; j++ )buff[j] = '\0' ;
            }
            if(ch == '\n')
            {
                buff[i++] = '\0' ;
                if( (strcmp("-1" , buff  ) == 0 ) && (exit_flag == 0 ))break ;
                send(sockfd, buff , 21 , 0);
                break ;
            }
                buff[i++] = ch ;
                ch = getchar() ;
        }
        char buff_rec[100] ;
        if( (strcmp("-1" , buff  ) == 0 ) && (exit_flag == 0 ))break ;
        recv(sockfd, buff_rec, 100 , 0);
        printf("Result from server : %s\n", buff_rec);
    }

    close(sockfd);
    return 0;
}
