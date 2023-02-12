/*
            NETWORK PROGRAMMING WITH SOCKETS

*** Vinod Meena 
*** 20CS10074
*** Network Lab : Assignment 2 
*** Problem 2 

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

	int i;
	char buf[50];

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

	printf("\nConnected to server\n");

	for (i = 0; i < 50; i++)
		buf[i] = '\0';
	recv(sockfd, buf, 50, 0);
	printf("%s", buf);

	char username[25];
	scanf("%s", username);

	strcpy(buf, username);
	send(sockfd, buf, 50, 0);

	for (i = 0; i < 50; i++)
		buf[i] = '\0';
	recv(sockfd, buf, 50, 0);
	printf("Status: %s\n", buf);

	if (strcmp(buf, "NOT FOUND") == 0)
	{
		close(sockfd);
		exit(1);
	}

	// printf("1994\n") ;
	// ch = getchar();
	while (1)
	{
		printf("Enter a shell command: ");
		char cmd[100];
		char ch = '\0';
		int k = 0;
		scanf(" %c", &ch);
		// scanf("%c", &ch);
		// printf("ch %c---",ch) ;

		while (ch != '\n')
		{
			cmd[k++] = ch;
			if (k == 50)
			{
				strcpy(buf, cmd);
				send(sockfd, buf, 50 , 0);
				k = 0;
			}
			// ch = getchar();
			scanf("%c", &ch);
		}
		// gets(cmd);
		cmd[k] = '\0';
		// printf("%s\n", cmd);
		if (strcmp(cmd, "exit") == 0)
		{
			printf("Successfully exit \n");
			break;
		}
		strcpy(buf, cmd);
		send(sockfd, buf, 50, 0);
		for( int j = 0 ; j< 50 ;j++)buf[j] = '\0' ;
		if (cmd[0] == 'c' && cmd[1] == 'd' && (cmd[2] == ' ' || strlen(cmd) == 2))
		{
			
			recv(sockfd, buf, 50, 0);
			// printf("%s 1\n" ,buf) ;
			if (strcmp(buf, "####") == 0)
			{
				printf("Error changing directory\n");
			}
			else
			{
				printf("%s\n",buf);
			}
		}
		else if (cmd[0] == 'p' && cmd[1] == 'w' && cmd[2] == 'd' && strlen(cmd) == 3)
		{
			// for (int i = 0; i < 50; i++)
			// 	buf[i] = '\0';
			char curr_dir[200];
			int flag = 0, ind = 0;
			while (flag == 0)
			{
				recv(sockfd, buf, 50, 0);
				// printf("%s pwd\n" , buf) ;
				for (int j = 0; j < 50; j++)
				{
					curr_dir[ind++] = buf[j];
					if (buf[j] == '\0')
					{
						flag = 1;
						break;
					}
					buf[j] = '\0';
				}
			}
			if (strcmp(curr_dir, "####") == 0)
			{
				perror("getcwd() error");
			}
			else
			{
                 printf("Current working directory: %s\n", curr_dir);

			}
				
		}
		else if (cmd[0] == 'd' && cmd[1] == 'i' && cmd[2] == 'r' &&  ( strlen(cmd) == 3  || cmd[3] == ' '))
		{
            // char curr_dir[200];
			int flag = 0;
			while (flag == 0)
			{
				recv(sockfd, buf, 50, 0);
				// printf("%s dir\n",buf) ;
				if( strcmp(buf , "####") == 0)break ;
				printf("%s",buf);
				for (int j = 0; j < 50; j++)
				{
					// curr_dir[ind++] = buf[j];
					if (buf[j] == '\0')
					{
						flag = 1;
						break;
					}
					buf[j] = '\0';
				}
			}
			if (strcmp(buf, "####") == 0)
			{
				printf("opendir() error\n");
			}
			printf("\n") ;
		}
		else
		{
			recv(sockfd, buf, 50, 0);
			// printf("%s 2\n",buf) ;
			if(strcmp( buf , "$$$$") == 0 )
			{
				printf("The given command is invalid\n") ;
			}
			// printf("Message from server : %s \n", buf);
		}
	}
	close(sockfd);
	return 0;
}
