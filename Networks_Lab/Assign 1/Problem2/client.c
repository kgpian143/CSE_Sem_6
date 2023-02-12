
/*    THE CLIENT PROCESS */

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
    /* Opening a socket is exactly similar to the server process */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Unable to create socket\n");
        exit(0);
    }

    /* Recall that we specified INADDR_ANY when we specified the server
       address in the server. Since the client can run on a different
       machine, we must specify the IP address of the server.

       In this program, we assume that the server is running on the
       same machine as the client. 127.0.0.1 is a special address
       for "localhost" (this machine)

    /* IF YOUR SERVER RUNS ON SOME OTHER MACHINE, YOU MUST CHANGE
           THE IP ADDRESS SPECIFIED BELOW TO THE IP ADDRESS OF THE
           MACHINE WHERE YOU ARE RUNNING THE SERVER.
        */

    serv_addr.sin_family = AF_INET;
    inet_aton("127.0.0.1", &serv_addr.sin_addr);
    serv_addr.sin_port = htons(20000);

    /* With the information specified in serv_addr, the connect()
       system call establishes a connection with the server process.
    */
    if ((connect(sockfd, (struct sockaddr *)&serv_addr,
                 sizeof(serv_addr))) < 0)
    {
        perror("Unable to connect to server\n");
        exit(0);
    }

    /* After connection, the client can send or receive messages.
       However, please note that recv() will block when the
       server is not sending and vice versa. Similarly send() will
       block when the server is not receiving and vice versa. For
       non-blocking modes, refer to the online man pages.
    */
    while (1)
    {
        char *buff ;
        int buff_cap = 10 , i = 0 , exit_flag = 0 ;
        buff = malloc( 10 * sizeof(char)) ;
        printf("Enter the expression : ");
        char ch ;
        ch = getchar() ;
        while(1)
        {
            if( i == buff_cap )
            {
                // printf("%s 89\n",buff) ;
                send(sockfd, buff, 11 , 0);
                i = 0 ;
                exit_flag = 1 ;
                for( int j = 0 ;j < 10 ; j++ )buff[j] = '\0' ;
            }
            if(ch == '\n')
            {
                //  printf("%s exit",buff) ;
                buff[i++] = '\0' ;
                // if( strcmp("-1" , buff ) == 0 )break ;
                if( (strcmp("-1" , buff  ) == 0 ) && (exit_flag == 0 ))break ;
                send(sockfd, buff , 10 , 0);
                break ;
            }
                buff[i++] = ch ;
                ch = getchar() ;
        }
        // send(sockfd, buf, strlen(buf) + 1, 0);
        char buff_rec[50] ;
         if( (strcmp("-1" , buff  ) == 0 ) && (exit_flag == 0 ))break ;
        recv(sockfd, buff_rec, 50 , 0);
        printf("Value of expression from server : %s\n", buff_rec);
    }

    close(sockfd);
    return 0;
}
