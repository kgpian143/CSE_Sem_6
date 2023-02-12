
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
	int load1 , load2 ;
	struct pollfd fds;
	fds.fd = sockfd1;
	fds.events = POLLIN;
	// int res = poll(&fds, 1, 3000);
	while (1)
	{
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
		strcpy(buf, "Send load");
		send(sockfd2, buf, 100, 0);
        for( int i = 0 ; i < 100 ; i++ )buf[i] = '\0' ;
		recv(sockfd2, buf, 100, 0);
		load1 = atoi(buf);
		printf("\nLoad received from server with port number %d : %d \n",ntohs(serv_addr1.sin_port), load1);
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
		strcpy(buf, "Send load");
		send(sockfd2, buf, 100, 0);
        for( int i = 0 ; i < 100 ; i++ )buf[i] = '\0' ;
		recv(sockfd2, buf, 100, 0);
		load2 = atoi(buf);
        printf("\nLoad received from server with port number %d : %d \n",ntohs(serv_addr2.sin_port), load2);
		close(sockfd2);
        // Start timer
        // start = clock();
		while (poll(&fds, 1, 5000) > 0)
		{
			clilen = sizeof(cli_addr);
			newsockfd = accept(sockfd1, (struct sockaddr *)&cli_addr,
							   &clilen);
			if (newsockfd < 0)
			{
				printf("Accept error1\n");
				exit(0);
			}
            for( int i = 0 ; i < 100 ; i++ )buf[i] = '\0' ;
			recv(newsockfd, buf, 100, 0);  
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
				if (load2 > load1)
				{
					if ((connect(sockfd2, (struct sockaddr *)&serv_addr1,
								 sizeof(serv_addr1))) < 0)
					{
						perror("Unable to connect to server\n");
						exit(0);
					}
                    printf("Sending client request server with port number %d \n" ,  ntohs(serv_addr1.sin_port) );
					strcpy(buf , "Send time");
					send(sockfd2, buf, 100, 0);
                    for( int i = 0 ; i < 100 ; i++ )buf[i] = '\0' ;
					recv(sockfd2, buf, 100, 0);
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
					strcpy(buf , "Send time");
					send(sockfd2, buf, 100, 0);
                    for( int i = 0 ; i < 100 ; i++ )buf[i] = '\0' ;
					recv(sockfd2, buf, 100, 0);
				}

				/* We again initialize the buffer, and receive a
				   message from the client.
				*/
				close(sockfd2);

				send(newsockfd, buf , 100, 0);
				close(newsockfd);
				exit(0) ;
			}
			close(newsockfd) ;
		}
		
	}
	return 0;
}
