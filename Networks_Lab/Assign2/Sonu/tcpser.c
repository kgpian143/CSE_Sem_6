/**
 * @file tcpsr.c
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
#include <dirent.h>

int main()
{
    int sockfd, newsockfd; 
    int clilen;
    struct sockaddr_in cli_addr, serv_addr;

    int i;
    char buf[200]; 

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Cannot create socket\n");
        exit(0);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(20000);

    if (bind(sockfd, (struct sockaddr *)&serv_addr,
             sizeof(serv_addr)) < 0)
    {
        printf("Unable to bind local address\n");
        exit(0);
    }

    listen(sockfd, 5); 

    printf("\nWaiting for client..\n");

    while (1)
    {
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,
                           &clilen);

        if (newsockfd < 0)
        {
            printf("Accept error\n");
            exit(0);
        }

        printf("\nClient Connected\n");

        if (fork() == 0)
        {
            close(sockfd); 

            strcpy(buf, "LOGIN:");
            send(newsockfd, buf, 50, 0);

            for (i = 0; i < 50; i++)
                buf[i] = '\0';
            recv(newsockfd, buf, 50, 0);
            // printf("Username is : %s\n", buf);

            FILE *fp;
            char word[50];
            char ch;
            char *filename = "users.txt";

            fp = fopen(filename, "r");
            int flag = 0;
            do
            {
                ch = fscanf(fp, "%s", word);
                if (strcmp(word, buf) == 0)
                {
                    // printf("Username found : %s\n", buf);

                    strcpy(buf, "FOUND");
                    send(newsockfd, buf, strlen(buf) + 1, 0);
                    flag++;
                    break;
                }
            } while (ch != EOF);

            fclose(fp);

            if (flag == 0)
            {
                strcpy(buf, "NOT FOUND");
                send(newsockfd, buf, strlen(buf) + 1, 0);
            }

            while (1)
            {
                for (i = 0; i < 200; i++)
                    buf[i] = '\0';

                int rv = recv(newsockfd, buf, 200, 0);
               
                if(rv==0)
                {
                    printf("Client Disconnected\n");
                    break;
                }

                // printf("Command is : %s\n", buf);

                if (buf[0] == 'c' && buf[1] == 'd' && ( buf[2] == ' ' || strlen(buf) == 2 ))
                {
                    char path[100];
                    if( strlen(buf) == 2 )strcpy(path , "/home") ;
                    else stpcpy(path, buf + 3);
                    int result;
                    result = chdir(path);
                    if (result == 0)
                    {
                        char ans[200] ; 
                        strcpy( ans ,"Directory changed to : ") ;
                                // printf("Directory changed successfully\n"); 
                        char cwd[1024];
                        getcwd(cwd, sizeof(cwd)) ;
                        strcat( ans , cwd );
                        send(newsockfd , ans , 200 , 0 ) ;
                    }
                    else
                    {
                        char err[200] ;
                        strcpy( err ,"####") ;
                                // printf("Directory changed successfully\n");
                        send(newsockfd , err , 200 , 0 ) ;
                    }

                }
                else if (buf[0] == 'p' && buf[1] == 'w' && buf[2] == 'd' && strlen(buf) == 3)
                {

                    char cwd[1024];
                    char ans[200] ;
                    if (getcwd(cwd, sizeof(cwd)) != NULL)
                    {
                        // printf("Current working directory: %s\n", cwd);
                        strcpy(ans, cwd);
                        send(newsockfd, ans, 200, 0); 
                        // printf("x->%d\n", x);
                        for (i = 0; i < 50; i++) buf[i] = '\0';
                    }
                    else
                    {
                        char err[200] ;
                        strcpy( err , "####") ;
                         send(newsockfd, err, 200, 0); 
                        // perror("getcwd() error");
                    }

                }
                else if (buf[0] == 'd' && buf[1] == 'i' && buf[2] == 'r' && ( strlen(buf) == 3 || buf[3] == ' '))
                {
                    DIR *dir; 
                    struct dirent *entry;
                    char cwd[1024];
                    if( strlen(buf) == 3 )
                    {
                        getcwd(cwd, sizeof(cwd));
                        dir = opendir(cwd);
                    }
                    else 
                    {
                        dir = opendir(buf + 4 ) ;
                    }
                    if (dir == NULL)
                    {
                         char err[200] ;
                         strcpy( err , "####") ;
                         send(newsockfd, err, 200, 0);

                        // return 1;
                        continue;
                    }
                    char ans[200] ; 
                    int flag3 = 0 ;
                    while ((entry = readdir(dir)) != NULL)
                    {
                        // printf("%s\n", entry->d_name);
                        // for (i = 0; i < 50; i++) buf[i] = '\0';
                        if(flag3 == 0 )strcpy(ans, entry->d_name);
                        else 
                        {
                            strcat( ans , entry->d_name ) ;
                        }
                        flag3 = 1;
                        strcat( ans , " ") ;
                        // send(newsockfd, buf, 50, 0); 
                        // printf("x->%d\n", x);
                        // for (i = 0; i < 50; i++) buf[i] = '\0';
                    }
                    // printf("%s",ans) ;
                    send(newsockfd, ans, 200 , 0); 
                    closedir(dir);

                }
                else
                {
                    strcpy(buf, "$$$$");
                    send(newsockfd, buf, 50, 0); 
                    // printf("x->%d\n", x);
                    // for (i = 0; i < 100; i++) buf[i] = '\0';

                }  
      
            }
            
            close(newsockfd);
            exit(0);
        }

        close(newsockfd);
    }
    return 0;
}
