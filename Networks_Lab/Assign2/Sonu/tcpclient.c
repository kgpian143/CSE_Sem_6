/**
 * @file tcpcl.c
 * @author Sonu Yadav 
 * @brief  20CS10061
 * @version 0.1
 * @date 2023-01-20
 * @category Network Assignment 2 : Problem 2
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
    printf("%s\n", buf);

    char name[26];
    scanf("%s", name);

    strcpy(buf, name);
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

    char c;
    scanf("%c", &c);
    while (1)
    {
        printf("Enter a shell command: ");
        char cmd[200];
        memset(cmd, 0, sizeof(cmd));
        i = 0;
        scanf("%c", &cmd[i]);
        while (cmd[i] != '\n')
        {
            i++;
            scanf("%c", &cmd[i]);
        }

        cmd[i] = '\0';

        if (strcmp(cmd, "Exit") == 0)
        {
            close(sockfd);
            exit(1);
        }

        strcpy(buf, cmd);
        send(sockfd, buf, 200, 0);

        int n = 0;
        int flag = 0;
        for (i = 0; i < 50; i++)
            buf[i] = '\0';
        char ans[200];
        for( int i = 0 ; i < 200 ; i++ )ans[i] = '\0' ;
        n = recv(sockfd, ans, 200, 0);
        //  if( n == 0 )break ;
        if (strcmp(ans, "$$$$") == 0)
        {
            printf("The given command is invalid\n");
        }
        else if (strcmp(ans, "####") == 0)
        {
            printf("The command is valid but it is not get correct argument \n");
        }

        else
        {
            printf("%s\n", ans);
        }
    }

    close(sockfd);
    return 0;
}
