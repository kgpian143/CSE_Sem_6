/**
 * @file server.c
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
#include<time.h>
/* THE SERVER PROCESS */

void send_message( char mess[1000] , int sockfd )
{
    int  ind1 = 0, ind2 = 0;
    char buf[5];
    while (1)
    {
        if (ind1 == 5)
        {
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
		// printf("%s ! %s \n",mess , buf) ;
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

int main(int argc, char **argv)
{
	srand(time(0));
	int sockfd, newsockfd; /* Socket descriptors */
	int clilen;
	struct sockaddr_in cli_addr, serv_addr;

	int i;
	char buf[100]; /* We will use this buffer for communication */

	/* The following system call opens a socket. The first parameter
	   indicates the family of the protocol to be followed. For internet
	   protocols we use AF_INET. For TCP sockets the second parameter
	   is SOCK_STREAM. The third parameter is set to 0 for user
	   applications.
	*/
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
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
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(atoi(argv[1]));

	/* With the information provided in serv_addr, we associate the server
	   with its port using the bind() system call.
	*/
	if (bind(sockfd, (struct sockaddr *)&serv_addr,
			 sizeof(serv_addr)) < 0)
	{
		perror("Unable to bind local address\n");
		exit(0);
	}

	listen(sockfd, 5); /* This specifies that up to 5 concurrent client
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
    
	while (1)
	{

		/* The accept() system call accepts a client connection.
		   It blocks the server until a client request comes.

		   The accept() system call fills up the client's details
		   in a struct sockaddr which is passed as a parameter.
		   The length of the structure is noted in clilen. Note
		   that the new socket descriptor returned by the accept()
		   system call is stored in "newsockfd".
		*/
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,
						   &clilen);

		if (newsockfd < 0)
		{
			perror("Accept error\n");
			exit(0);
		}
        // printf("Client connected\n") ;
		/* We initialize the buffer, copy the message to it,
			and send the message to the client.
		*/
		// recv(newsockfd, buf, 100, 0);
		char mess[1000] ;
		receive_message( mess , newsockfd ) ;
		// printf("%s\n", mess);
		if (strcmp(mess, "Send load") == 0)
		{
			// time_t t1;
			int val = rand() % 100;
			// strcpy(buf,"load sent : ");
			char res[1000];
            printf("Load sent : %d\n",val) ;
			sprintf(res, "%d", val);
			// strcpy(buf, res);
			// send(newsockfd, buf, strlen(buf) + 1, 0);
			send_message( res , newsockfd ) ;
		}

		else if (strcmp(mess , "Send time") == 0)
		{
			time_t rawtime ;
			struct tm *timeinfo;
			time(&rawtime);
			timeinfo = localtime(&rawtime);
            // printf("Time send to client : %s \n",asctime(timeinfo)) ;
			char time_message[1000] ;
			strcpy(time_message, "Time of connection  :- ");
			strcat(time_message, asctime(timeinfo));
			// printf("%s",time_message) ;
			send_message(time_message , newsockfd);
		}

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

		close(newsockfd);
	}
	return 0;
}
