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
#include <dirent.h>
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

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(20000);

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
    printf("\nWaiting for client..\n");

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

        printf("\nClient Connected\n");

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

            strcpy(buf, "LOGIN:");
            send(newsockfd, buf, strlen(buf) + 1, 0);

            /* We again initialize the buffer, and receive a
               message from the client.
            */
            for (i = 0; i < 50; i++)
                buf[i] = '\0';
            recv(newsockfd, buf, 50, 0);
            // printf("Username is : %s\n", buf);

            FILE *fp;
            char word[50];
            char ch;
            char *filename = "users.txt";

            /*  open for writing */
            fp = fopen(filename, "r");
            int flag = 0;
            do
            {
                ch = fscanf(fp, "%s", word);
                if (strcmp(word, buf) == 0)
                {
                    // printf("Username found : %s\n", buf);

                    strcpy(buf, "FOUND");
                    send(newsockfd, buf, 50, 0);
                    flag=1;
                    break;
                }
            } while (ch != EOF);

            fclose(fp);

            if (flag == 0)
            {
                strcpy(buf, "NOT FOUND");
                send(newsockfd, buf, 50, 0);
            }
            for( int i = 0 ; i < 50 ; i++ )buf[i] = '\0' ;
            while ( recv(newsockfd, buf, 50, 0))
            {
                char cmd[1000];
                strcpy(cmd, buf);
                int flag1 = 0;
                for (int i = 0; i < 50; i++)
                {
                    if (buf[i] == '\0')
                    {
                        flag1 = 1;
                        break;
                    }
                    buf[i] = '\0';
                }
                // cmd[0] = '\0';

                while (flag1 == 0)
                {
                    recv(newsockfd, buf, 51, 0);
                    strcat(cmd, buf);
                    for (int i = 0; i < 50; i++)
                    {
                        if (buf[i] == '\0')
                        {
                            flag1 = 1;
                            break;
                        }
                        buf[i] = '\0';
                    }
                }

                // recv(newsockfd, buf, 100, 0);
                // printf("Command from client is : %s\n", cmd);

                //
                if (cmd[0] == 'c' && cmd[1] == 'd' && (cmd[2] == ' ' || strlen(cmd) == 2))
                {
                    // printf("....\n");
                    char path[100];
                    if( strlen(cmd) == 2 )
                    {
                        strcpy( path , "/home/vinod") ;
                    }
                    else stpcpy(path, cmd + 3);
                    int result;
                    result = chdir(path);
                    // printf("result :%d \n",result) ;
                    if (result == 0)
                    {
                        // printf("Directory changed successfully\n");
                        strcpy(buf, "Directory changed successfully\n");
                        send(newsockfd, buf, 50, 0);
                    }
                    else
                    {
                        // printf("Error changing directory\n");
                        strcpy(buf, "####");
                        send(newsockfd, buf, 50, 0);
                    }
                }
                else if (cmd[0] == 'p' && cmd[1] == 'w' && cmd[2] == 'd' && ( strlen(cmd) == 3 ))
                {

                    char cwd[200];
                    if (getcwd(cwd, sizeof(cwd)) != NULL)
                    {
                        int k = 0, j = 0;
                        while (cwd[k] != '\0')
                        {
                            if (j == 50)
                            {
                                send(newsockfd, buf, 50, 0);
                                j = 0;
                            }
                            buf[j++] = cwd[k++];
                        }
                        buf[j] = '\0';
                        send(newsockfd, buf, 50, 0);
                    }
                    else
                    {
                        // perror("getcwd() error");
                        strcpy(buf, "####");
                        send(newsockfd, buf, 50, 0);
                        // return 1;
                    }
                }
                else if (cmd[0] == 'd' && cmd[1] == 'i' && cmd[2] == 'r' && ( strlen(cmd) == 3  || cmd[3] == ' '))
                {
                    DIR *dir;
                    struct dirent *file_name;
                    char cwd[200];
                    if( strlen(cmd) == 3 )
                    {
                        getcwd(cwd, sizeof(cwd));
                        dir = opendir(cwd);
                    }
                    else 
                    {
                        dir = opendir(cmd + 4) ;
                    }
                    if (dir == NULL)
                    {
                        strcpy(buf, "####");
                        send(newsockfd, buf, 50, 0);
                        // perror("opendir() error");
                        // return 1;
                    }
                    else
                    {
                        int ind = 0 ;
                        while ((file_name = readdir(dir)) != NULL)
                        {
                            // strcpy(buf , file_name->d_name) ;
                            // send(newsockfd, buf, 50, 0);
                            for( int j = 0 ; file_name->d_name[j] != '\0' ; j++ )
                            {
                                if( ind == 50 )
                                {
                                   send(newsockfd, buf, 50, 0);
                                   ind = 0 ; 
                                }
                                buf[ind++] = file_name->d_name[j] ;
                            }
                            // printf("%s\n", file_name->d_name);
                            if( ind < 50 )
                            {
                                buf[ind++] = ' ';
                            }
                            else 
                            {
                                send(newsockfd, buf, 50, 0);
                                ind = 0 ;
                                buf[ind++] = ' ' ;
                            }
                        }
                        if( ind < 50 )
                        {
                            buf[ind++] = '\0' ;
                            send(newsockfd, buf, 50, 0);
                        }
                        else 
                        {
                            send(newsockfd, buf, 50, 0);
                            ind = 0 ;
                            buf[ind++] = '\0' ;
                            send(newsockfd, buf, 50, 0);
                        }
                    }

                    closedir(dir);
                    // system("dir")  ;
                    // return 0;
                }
                else
                {
                    strcpy(buf , "$$$$") ;
                    send(newsockfd, buf, 50, 0);
                    // printf("Invalid command\n");
                }
            }
            printf("One Client disconnected\n") ;
            close(newsockfd);
            exit(0);
        }

        close(newsockfd);
    }
    return 0;
}
