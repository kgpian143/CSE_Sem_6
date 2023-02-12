/**
 * @file lb.c
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
#include <poll.h>
#include <time.h>
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
	int sockfd1, newsockfd, sockfd2; /* Socket descriptors */
	int clilen;
	struct sockaddr_in cli_addr, serv_addr1, serv_addr2, lb_addr;
	int i;
	char buf[100]; /* We will use this buffer for communication */

	/* The following system call opens a socket. The first parameter
	   indicates the family of the protocol to be followed. For internet
	   protocols we use AF_INET. For TCP sockets the second parameter
	   is SOCK_STREAM. The third parameter is set to 0 for user
	   applications.
	*/
	if ((sockfd1 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("Cannot create socket\n");
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

	lb_addr.sin_family = AF_INET;
	lb_addr.sin_addr.s_addr = INADDR_ANY;
	lb_addr.sin_port = htons(atoi(argv[1]));

	serv_addr1.sin_family = AF_INET;
	inet_aton("127.0.0.1", &serv_addr1.sin_addr);
	serv_addr1.sin_port = htons(atoi(argv[2]));

	serv_addr2.sin_family = AF_INET;
	inet_aton("127.0.0.1", &serv_addr2.sin_addr);
	serv_addr2.sin_port = htons(atoi(argv[3]));

	/* With the information provided in serv_addr, we associate the server
	   with its port using the bind() system call.
	*/
	if (bind(sockfd1, (struct sockaddr *)&lb_addr,
			 sizeof(lb_addr)) < 0)
	{
		printf("Unable to bind local address\n");
		exit(0);
	}
    char server_ip2[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(serv_addr2.sin_addr), server_ip2, INET_ADDRSTRLEN);
	char server_ip1[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(serv_addr1.sin_addr), server_ip1, INET_ADDRSTRLEN);

	int port1 = ntohs(serv_addr1.sin_port) ;
	int port2 = ntohs(serv_addr2.sin_port) ;
	listen(sockfd1, 5);
	/* This specifies that up to 5 concurrent client
				  requests will be queued up while the system is
				  executing the "accept" system call below.
			   */

	/* In this program we are illustrating a concurrent server -- one
	   which forks to accept multiple client connections concurrently.
	   As soon as the server accepts a connection from a client, it
	   forks a child which communicates with the client, while the
	   parent becomes free to accept a new connection. To facilitate
	   this, the accept() system call returns a new socket descriptor
	   which can be used by the child. The parent continues with the
	   original socket descriptor.
	*/
	int load1 = 0, load2 = 0;
	struct pollfd fds;
	fds.fd = sockfd1;
	fds.events = POLLIN;
	// int res = poll(&fds, 1, 3000);
	while (1)
	{
		char rec_load1[1000], rec_load2[1000];
		if ((sockfd2 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			perror("Unable to create socket\n");
			exit(0);
		}

		if ((connect(sockfd2, (struct sockaddr *)&serv_addr1,
					 sizeof(serv_addr1))) < 0)
		{
			perror("Unable to connect to server1\n");
			exit(0);
		}
		char mess[1000] ;
		strcpy(mess, "Send load");
		// send(sockfd2, buf, 50, 0);
		send_message( mess , sockfd2) ;
		receive_message( rec_load1 , sockfd2) ;
		// recv(sockfd2, rec_load1, 100, 0);
		load1 = atoi(rec_load1);
		printf("\nLoad received from  address '%s' with port number %d is %d \n", server_ip1 , port1, load1);
		close(sockfd2);
		if ((sockfd2 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			perror("Unable to create socket\n");
			exit(0);
		}

		if ((connect(sockfd2, (struct sockaddr *)&serv_addr2,
					 sizeof(serv_addr2))) < 0)
		{
			perror("Unable to connect to server1\n");
			exit(0);
		}
		// strcpy(buf, "Send load");
		send_message( mess , sockfd2) ;
		receive_message( rec_load2 , sockfd2) ;;
		load2 = atoi(rec_load2);
		printf("\nLoad received from address '%s' with port number %d is %d \n",server_ip2 , port2, load2);
		close(sockfd2);
		clock_t start, end;
        double diff = 5 ; 
        // Start timer
        // start = clock();
        int cnt = 0 ;

		while (poll(&fds, 1, diff *  1000) > 0)
		{
			   start = clock() ;
			/* The accept() system call accepts a client connection.
			   It blocks the server until a client request comes.

			   The accept() system call fills up the client's details
			   in a struct sockaddr which is passed as a parameter.
			   The length of the structure is noted in clilen. Note
			   that the new socket descriptor returned by the accept()
			   system call is stored in "newsockfd".
			*/
		    if(diff <= 0 )break ;
		    // printf("%d\n",cnt++) ;
            // printf("diff : %f \n", diff) ;
			clilen = sizeof(cli_addr);
			newsockfd = accept(sockfd1, (struct sockaddr *)&cli_addr,
							   &clilen);

			if (newsockfd < 0)
			{
				printf("Accept error1\n");
				exit(0);
			}
			char req_client[1000];
			recv(newsockfd, req_client, 100, 0);
			/* Having successfully accepted a client connection, the
			   server now forks. The parent closes the new socket
			   descriptor and loops back to accept the next connection.
			*/
			if (fork() == 0)
			{

				/* This child process will now communicate with the
				   client through the send() and recv() system calls.
				*/
				close(sockfd1); /* Close the old socket since all
						   communications will be through
						   the new socket.
						*/

				/* We initialize the buffer, copy the message to it,
				   and send the message to the client.
				*/
				if ((sockfd2 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
				{
					perror("Unable to create socket\n");
					exit(0);
				}
				char time_mess[1000];
				if (load2 > load1)
				{
					if ((connect(sockfd2, (struct sockaddr *)&serv_addr1,
								 sizeof(serv_addr1))) < 0)
					{
						perror("Unable to connect to server\n");
						exit(0);
					}
					char req_mess[1000] ;
                    printf("Sending client request to address '%s' with port number %d \n" , server_ip1 , port1 );
					strcpy(req_mess , "Send time");
					// send(sockfd2, buf, 50, 0);
					send_message( req_mess , sockfd2 ) ;
					// char res[1000];
					// recv(sockfd2, res, 100, 0);
					receive_message( time_mess , sockfd2) ;
					// printf("Current time from load %d : %s \n", load1 , res);
				}
				else
				{
					if ((connect(sockfd2, (struct sockaddr *)&serv_addr2,
								 sizeof(serv_addr2))) < 0)
					{
						perror("Unable to connect to server\n");
						exit(0);
					}
                    printf("Sending client request to address '%s' with port number %d \n" , server_ip2 , port2 );
					char req_mess[1000] ;
					strcpy(req_mess, "Send time");
					send_message( req_mess , sockfd2 ) ;
					receive_message( time_mess , sockfd2) ;
					// printf("Current time from load %d : %s \n", server_ip2 , port2);
				}

				/* We again initialize the buffer, and receive a
				   message from the client.
				*/
				close(sockfd2);

				// send(newsockfd, time_mess , 100, 0);
				send_message(time_mess , newsockfd ) ;
				close(newsockfd);
				exit(0) ;
			}
			end = clock();

    // Calculate time difference
            diff -= ((double)(end - start) / CLOCKS_PER_SEC)*10000;
			close(newsockfd) ;
		}
		
	}
	return 0;
}
