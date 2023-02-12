
/**
 * @file client.c
 * @author Vinod  Meena (vinodmeena.iitkgp@gmail.com)
 * @brief   Network Lab Assignment 3
 * @version 0.1
 * @date 2023-01-27
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
void send_message( char mess[1000] , int sockfd )
{
    int  ind1 = 0, ind2 = 0;
    char buf[5];
    while (1)
    {
        if (ind1 == 5)
        {
            // printf("%s uhiu \n",buf) ;
            send(sockfd, buf, 5, 0);
            ind1 = 0;
        }
        else if (mess[ind2] == '\0')
		{
            buf[ind1++] = '\0';
            send(sockfd, buf, 5, 0);
			break;
		}
            
        buf[ind1++] = mess[ind2++];
    }
}

void receive_message( char mess[1000] , int id )
{
	char buf[10] ; 
    mess[0] = '\0' ;
	int k , j , flag =0 ; 
	for( int i = 0 ; i < 10 ; i++ )buf[i] = '\0' ;
	while(1)
	{
        recv( id , buf  , 5 , 0 ) ;
        // printf(" %s 0 %s \n",mess , buf) ;
		strcat( mess , buf ) ;
		for( int i = 0 ; i < 5 ; i++ )
		{
			if( buf[i] == '\0')
			{
				flag =1 ;
				break; 
			}
            buf[i] = '\0' ;
		}
		if(flag)break;
	}
}
int main( int argc , char **argv)
{
	int			sockfd ;
	struct sockaddr_in	serv_addr;

	int i;
	char buf[100];

	/* Opening a socket is exactly similar to the server process */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
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

	serv_addr.sin_family	= AF_INET;
	inet_aton("127.0.0.1", &serv_addr.sin_addr);
	serv_addr.sin_port	= htons(atoi(argv[1]));

	/* With the information specified in serv_addr, the connect()
	   system call establishes a connection with the server process.
	*/
	if ((connect(sockfd, (struct sockaddr *) &serv_addr,
						sizeof(serv_addr))) < 0) {
		perror("Unable to connect to server\n");
		exit(0);
	}
    // printf("client connected to server") ;
	/* After connection, the client can send or receive messages.
	   However, please note that recv() will block when the
	   server is not sending and vice versa. Similarly send() will
	   block when the server is not receiving and vice versa. For
	   non-blocking modes, refer to the online man pages.
	*/
    char req_mess[1000] ;
    strcpy(req_mess,"Send time");
	// send(sockfd, buf, strlen(buf) + 1, 0);
	send_message(req_mess , sockfd ) ;
	// for(i=0; i < 100; i++) buf[i] = '\0';
	char time_mess[1000] ;
	receive_message( time_mess , sockfd) ;
	// recv(sockfd, buf, 100, 0);
	printf("%s\n", time_mess );	
	close(sockfd);
	return 0;

}

