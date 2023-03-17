
/*    THE CLIENT PROCESS */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void send_message( char mess[100] , int sockfd )
{
    int  ind1 = 0, ind2 = 0;
    char buf[10];
    while (1)
    {
        if (ind1 == 10)
        {
            // printf("%s uhiu \n",buf) ;
            send(sockfd, buf, 10, 0);
            ind1 = 0;
        }
        else if (mess[ind2] == '\0')
		{
            buf[ind1++] = '\0';
            send(sockfd, buf, 10, 0);
			break;
		}
            
        buf[ind1++] = mess[ind2++];
    }
}

void receive_message( char mess[100] , int id )
{
	char buf[10] ; 
    mess[0] = '\0' ;
	int k , j , flag =0 ; 
	for( int i = 0 ; i < 10 ; i++ )buf[i] = '\0' ;
	while(1)
	{
        recv( id , buf  , 10 , 0 ) ;
        // printf(" %s 0 %s \n",mess , buf) ;
		strcat( mess , buf ) ;
		for( int i = 0 ; i < 10 ; i++ )
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
void my_send(char message[100], int sockfd)
{
	char buf[10];
	int j = 0;
	for (int i = 0; i < 100; i++)
	{
		if(message[i] == '\0')break;
		if (j == 9)
		{
			send(sockfd, buf, 10, 0);
			printf("%s ser\n",buf) ;
			for (int k = 0; k < 10; k++)
				buf[k] = '\0';
			j = 0 ;
		}
		buf[j++] = message[i];
	}
	send(sockfd, buf, 10, 0);
	printf("%s ser\n",buf) ;
	for (int k = 0; k < 10; k++)
		buf[k] = '\0';
	send(sockfd, buf, strlen(buf), 0);
}
void my_rec(char message[100], int sockfd)
{
	// int sockfd = atoi((char *)arg);
	char buf[10];
	// char message[5005];
	while (1)
	{
		int n = recv(sockfd, buf, 10, 0);
		printf("%s cli\n",buf) ;
		printf("%d\n",n);
		if (n == 0)
			break;
		strcat(message, buf);
		for (int i = 0; i < 10; i++)
			buf[i] = '\0';
		if( strlen(buf) == 100 )break ;
	}
}
int main()
{
	int sockfd;
	struct sockaddr_in serv_addr;

	int i;
	char buf[100];

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
	for (i = 0; i < 100; i++)
		buf[i] = '\0';
	receive_message( buf , sockfd );
	printf("%s\n", buf);

	strcpy(buf, "Message from client");
	send_message(buf , sockfd );

	close(sockfd);
	return 0;
}
