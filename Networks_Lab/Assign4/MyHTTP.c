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
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/stat.h>
#include <errno.h>
#include <stddef.h>

/* THE SERVER PROCESS */

int main()
{
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
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(20000);

	/* With the information provided in serv_addr, we associate the server
	   with its port using the bind() system call.
	*/
	if (bind(sockfd, (struct sockaddr *)&serv_addr,
			 sizeof(serv_addr)) < 0)
	{
		printf("Unable to bind local address\n");
		exit(0);
	}

	listen(sockfd, 5); /* This specifies that up to 5 concurrent client
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
			printf("Accept error\n");
			exit(0);
		}
		printf("Client connected \n");
		/* Having successfully accepted a client connection, the
		   server now forks. The parent closes the new socket
		   descriptor and loops back to accept the next connection.
		*/
		if (fork() == 0)
		{

			/* This child process will now communicate with the
			   client through the send() and recv() system calls.
			*/
			close(sockfd); /* Close the old socket since all
					  communications will be through
					  the new socket.
				   */

			/* We initialize the buffer, copy the message to it,
			   and send the message to the client.
			*/

			// strcpy(buf,"Message from server");
			// send(newsockfd, buf, strlen(buf) + 1, 0);

			// /* We again initialize the buffer, and receive a
			//    message from the client.
			// */
			// for(i=0; i < 100; i++) buf[i] = '\0';
			// recv(newsockfd, buf, 100, 0);
			// printf("%s\n", buf);

			char method[100];
			recv(newsockfd, method, 100, 0);

			char url[100];
			recv(newsockfd, url, 100, 0);
			printf("%s\n", url);
			char protocol_version[100];
			recv(newsockfd, protocol_version, 100, 0);
			printf("%s\n", protocol_version);
			char Host[100];
			recv(newsockfd, Host, 100, 0);
			printf("%s\n", Host);
			char Connection[100];
			recv(newsockfd, Connection, 100, 0);
			printf("%s\n", Connection);
			char Date[100];
			recv(newsockfd, Date, 100, 0);
			printf("%s\n", Date);
			char Accept[100];
			recv(newsockfd, Accept, 100, 0);
			printf("%s\n", Accept);
			char Accept_Language[100];
			recv(newsockfd, Accept_Language, 100, 0);
			printf("%s\n", Accept_Language);
			char If_Modified_Since[100];
			recv(newsockfd, If_Modified_Since, 100, 0);
			printf("%s\n", If_Modified_Since);

			if (strcmp(method, "GET") == 0)
			{
				// char url[100];
				// recv(newsockfd, url, 100, 0);
				// printf("%s\n", url);
				// char protocol_version[100];
				// recv(newsockfd, protocol_version, 100, 0);
				// printf("%s\n", protocol_version);
				// char Host[100];
				// recv(newsockfd, Host, 100, 0);
				// printf("%s\n", Host);
				// char Connection[100];
				// recv(newsockfd, Connection, 100, 0);
				// printf("%s\n", Connection);
				// char Date[100];
				// recv(newsockfd, Date, 100, 0);
				// printf("%s\n", Date);
				// char Accept[100];
				// recv(newsockfd, Accept, 100, 0);
				// printf("%s\n", Accept);
				// char Accept_Language[100];
				// recv(newsockfd, Accept_Language, 100, 0);
				// printf("%s\n", Accept_Language);
				// char If_Modified_Since[100];
				// recv(newsockfd, If_Modified_Since, 100, 0);
				// printf("%s\n", If_Modified_Since);

				// char content_Language[100] ;
				// recv( newsockfd , content_Language  , 100 , 0 ) ;
				// printf("%s\n",If_Modified_Since) ;
				// char content_Language[100] ;
				// recv( newsockfd , If_Modified_Since  , 100 , 0 ) ;
				// printf("%s\n",If_Modified_Since) ;
				char file_path[1000];
				// getcwd(file_path, 1000);
				// strcat(file_path,"/") ;
				strcpy(file_path, url);
				printf("file_path : %s\n", file_path);
				send(newsockfd, "HTTP/1.1", 100, 0);
				FILE *fp = fopen(file_path, "r");
				if (fp != NULL)
				{
					char buffer[100];
					int length;

					// Send HTTP header
					// printf("Yes");
					sprintf(buffer, "200");
					send(newsockfd, buffer, 100, 0);
					send(newsockfd, "OK", 100, 0);
					time_t rawtime, modified_time;
					struct tm *timeinfo;
					char buffer_time[80];

					time(&rawtime);
					modified_time = rawtime + (3 * 24 * 60 * 60);
					timeinfo = gmtime(&modified_time);

					strftime(buffer_time, 80, "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
					// sprintf(buffer, "Content-Type: application/octet-stream\r\n");
					send(newsockfd, buffer_time, 100, 0);
					// printf("expires time %s\n", buffer_time);
					send(newsockfd, "Content Lang : en-us\n", 100, 0);
					sprintf(buffer, "\r\n");
					send(newsockfd, buffer, strlen(buffer), 0);

					// Send the file contents
					while ((length = fread(buffer, 1, 100, fp)) > 0)
					{
						send(newsockfd, buffer, length, 0);
					}

					// Close file and client socket
					fclose(fp);
				}
				else
				{
					// If file not found, send 404 error
					char buffer[100];
					sprintf(buffer, "404");
					send(newsockfd, buffer, 100, 0);
					// sprintf(buffer, "HTTP/1.1 404 Not Found\r\n");
					// send(newsockfd, buffer, strlen(buffer), 0);
					// sprintf(buffer, "Content-Type: text/html\r\n");
					// send(newsockfd, buffer, strlen(buffer), 0);
					// sprintf(buffer, "\r\n");
					// send(newsockfd, buffer, strlen(buffer), 0);
					// sprintf(buffer, "<html><body><h1>404 Not Found</h1></body></html>\r\n");
					// send(newsockfd, buffer, strlen(buffer), 0);
				}
			}
			else if (strcmp(method, "PUT") == 0)
			{
				char content_language[100];
				recv(newsockfd, content_language, 100, 0);
				printf("%s\n", content_language);

				char content_length[100];
				recv(newsockfd, content_length, 100, 0);
				printf("%s\n", content_length);

				char content_type[100];
				recv(newsockfd, content_type, 100, 0);
				printf("%s\n", content_type);

				char file_name[100];
				recv(newsockfd, file_name, 100, 0);
				printf("%s\n", file_name);
                send(newsockfd, "HTTP/1.1", 100, 0);
				char path[1000];
				getcwd(path, sizeof(path));
				int result;
				result = chdir(url);
                
				if (result == 0)
				{
					FILE *fp = fopen(file_name, "w");
					char line[8196];
					if (fp != NULL)
					{
						char status_code[100] ; 
						strcpy( status_code , "200" ) ;
						send( newsockfd , status_code , 100 , 0 ) ; 
						char status_message[100] ; 
						strcpy( status_message , "OK" ) ;
						send( newsockfd , status_message , 100 , 0 ) ; 
						int read_size;
						while ((read_size = recv(sockfd, line, 8196, 0)) > 0)
						{
							fwrite(line, 1, read_size, fp);
						}
						fclose(fp);
					}
					else 
					{
						char status_code[100] ; 
						strcpy( status_code , "400" ) ;
						send( newsockfd , status_code , 100 , 0 ) ; 

					}
					chdir(path);
					// getcwd(path, sizeof(path));
					// strcat(path, url);
				}
				else
				{
					char status_code[100];
					strcpy(status_code, "404");
					send(newsockfd, status_code , 100, 0);
				}

				// FILE *fp = fopen(file_name, "w");
				// char line[8196];
				// if (fp != NULL)
				// {
				// 	int read_size;
				// 	while ((read_size = recv(sockfd, line, 8196, 0)) > 0)
				// 	{
				// 		fwrite(line, 1, read_size, fp);
				// 	}
				// 	fclose(fp);
				// }
			}

			close(newsockfd);
			exit(0);
		}

		close(newsockfd);
	}
	return 0;
}
