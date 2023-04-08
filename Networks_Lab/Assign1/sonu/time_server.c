#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    int sockfd , newsockfd ;
     int clilen  ;
     struct sockaddr_in cli_addr , serv_addr ;
      int  i;
      char buf[100] ;
      	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Cannot create socket\n");
		exit(0);
	}
    printf("****** Socket created *****\n\n Waiting for client .....\n");
    serv_addr.sin_family = AF_INET ;
    serv_addr.sin_addr.s_addr = INADDR_ANY ;
    serv_addr.sin_port = htons(20000) ;
    
    if(bind( sockfd , (struct sockaddr *)&serv_addr , sizeof(serv_addr)) < 0 )
    {
        perror("Unable to bind local address\n");
        exit(0);
    }
    listen( sockfd , 5 ) ;
    while(1)
    {
        clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,
						   &clilen);

		if (newsockfd < 0)
		{
			perror("Accept error\n");
			exit(0);
		}
        printf("Client Joined successfully \n") ;
        time_t rawtime;
		struct tm *timeinfo;
        time(&rawtime);
		timeinfo = localtime(&rawtime);
		printf("Current Time : %s\n", asctime(timeinfo));
		strcpy(buf, asctime(timeinfo) );
		send(newsockfd, buf, strlen(buf) + 1, 0);
    }

}