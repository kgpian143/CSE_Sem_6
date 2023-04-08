/*
			NETWORK PROGRAMMING WITH SOCKETS

In this program we illustrate the use of Berkeley sockets for interprocess
communication across the network. We show the communication between a server
process and a client process.


*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>

			/* THE SERVER PROCESS */
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
			// printf("%s ser\n",buf) ;
			for (int k = 0; k < 10; k++)
				buf[k] = '\0';
			j = 0 ;
		}
		buf[j++] = message[i];
	}
	send(sockfd, buf, 10, 0);
	// printf("%s ser\n",buf) ;
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
		// printf("%s cli\n",buf) ;
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
	int			sockfd, newsockfd ; /* Socket descriptors */
	int			clilen;
	struct sockaddr_in	cli_addr, serv_addr;

	int i;
	char buf[100];		/* We will use this buffer for communication */

	/* The following system call opens a socket. The first parameter
	   indicates the family of the protocol to be followed. For internet
	   protocols we use AF_INET. For TCP sockets the second parameter
	   is SOCK_STREAM. The third parameter is set to 0 for user
	   applications.
	*/
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Cannot create socket\n");
		exit(0);
	}

	/* The structure "sockaddr_in" is defined in <netinet/in.h> for the
	   internet family of protocols. This has three main fields. The
 	   field "sin_family" specifies the family and is therefore AF_INET
	   for the internet family. The field "sin_addr" specifies the
	   internet address of the server. This field is set to INADDR_ANY
	   for machines having a single IP address. The field "sin_port"
	   specifies the port number of the server.
	*/
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= INADDR_ANY;
	serv_addr.sin_port		= htons(20000);

	/* With the information provided in serv_addr, we associate the server
	   with its port using the bind() system call. 
	*/
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
					sizeof(serv_addr)) < 0) {
		perror("Unable to bind local address\n");
		exit(0);
	}

	listen(sockfd, 10); /* This specifies that up to 10 concurrent client
			      requests will be queued up while the system is
			      executing the "accept" system call below.
			   */

	/* In this program we are illustrating an iterative server -- one
	   which handles client connections one by one.i.e., no concurrency.
	   The accept() system call returns a new socket descriptor
	   which is used for communication with the server. After the
	   communication is over, the process comes back to wait again on
	   the original socket descriptor.
	*/
	while (1) {

		/* The accept() system call accepts a client connection.
		   It blocks the server until a client request comes.

		   The accept() system call fills up the client's details
		   in a struct sockaddr which is passed as a parameter.
		   The length of the structure is noted in clilen. Note
		   that the new socket descriptor returned by the accept()
		   system call is stored in "newsockfd".
		*/
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,
					&clilen) ;

		if (newsockfd < 0) {
			perror("Accept error\n");
			exit(0);
		}


		/* We initialize the buffer, copy the message to it,
			and send the message to the client. 
		*/
		
		strcpy(buf,"Message from server");
		send_message(buf , newsockfd );

		/* We now receive a message from the client. For this example
  		   we make an assumption that the entire message sent from the
  		   client will come together. In general, this need not be true
		   for TCP sockets (unlike UDPi sockets), and this program may not
		   always work (for this example, the chance is very low as the 
		   message is very short. But in general, there has to be some
	   	   mechanism for the receiving side to know when the entire message
		  is received. Look up the return value of recv() to see how you
		  can do this.
		*/ 
		receive_message (buf , newsockfd );
		printf("%s\n", buf);

		close(newsockfd);
	}
	return 0;
}
			

