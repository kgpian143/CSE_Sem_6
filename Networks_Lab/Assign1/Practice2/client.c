
/*    THE CLIENT PROCESS */
/*
*******Networks Lab*******
* Name : Vinod Meena 
* Roll Number : 20CS10074
*/

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

    serv_addr.sin_family = AF_INET;
    inet_aton("127.0.0.1", &serv_addr.sin_addr);
    serv_addr.sin_port = htons(20000);

    if ((connect(sockfd, (struct sockaddr *)&serv_addr,
                 sizeof(serv_addr))) < 0)
    {
        perror("Unable to connect to server\n");
        exit(0);
    }

   
    while (1)
    {
        char buff[10] ;
        int buff_cap = 10 , i = 0 , exit_flag = 0 ;
        printf("Enter the expression : ");
        char ch ;
        ch = getchar() ;
        while(1)
        {
            if( i == buff_cap )
            {
                send(sockfd, buff, 11 , 0);
                i = 0 ;
                exit_flag = 1 ;
                for( int j = 0 ; j < 10 ; j++ )buff[j] = '\0' ;
            }
            if(ch == '\n')
            {
                buff[i++] = '\0' ;
                 if( (strcmp("-1" , buff  ) == 0 ) && (exit_flag == 0 ))break ;
                send(sockfd, buff , 10 , 0);
                break ;
            }
                buff[i++] = ch ;
                ch = getchar() ;
        }
        char rec_ans[100] ;
          if( (strcmp("-1" , buff  ) == 0 ) && (exit_flag == 0 ))break ;
        recv(sockfd, rec_ans, 100 , 0);
        printf("Value of expression from server : %s\n", rec_ans);
    }

    close(sockfd);
    return 0;
}
