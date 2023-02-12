// NETWORKS LAB (Assignent: 01, Qn: 01, SERVER PROCESS)
// Name: Kriti Bhardwaj
// Roll No: 20CS30028

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <time.h>
#include <poll.h>

#define CHUNK_SIZE 10

void send_str(int sockfd, char str[20000])
{
    int flag = 0, ind1 = 0, ind2 = 0;
    char buf[CHUNK_SIZE];
    while (1)
    {
        if (ind1 == CHUNK_SIZE)
        {
            send(sockfd, buf, CHUNK_SIZE, 0);
            ind1 = 0;
        }
        if (str[ind2] == '\0')
            break;
        buf[ind1++] = str[ind2++];
    }
    if (ind1 == CHUNK_SIZE)
    {
        send(sockfd, buf, CHUNK_SIZE, 0);
        ind1 = 0;
        buf[ind1++] = '\0';
        send(sockfd, buf, CHUNK_SIZE, 0);
    }
    else
    {
        buf[ind1++] = '\0';
        send(sockfd, buf, CHUNK_SIZE, 0);
    }
}

void recv_str(int sockfd, char str[20000])
{
    int flag = 0;
    char buf[100];
    for (int i = 0; i < 100; i++)
        buf[i] = '\0';
    recv(sockfd, buf, CHUNK_SIZE, 0);
    strcpy(str, buf);
    for (int i = 0; i < CHUNK_SIZE; i++)
    {
        if (buf[i] == '\0')
        {
            flag = 1;
            break;
        }
    }
    while (flag == 0)
    {
        for (int i = 0; i < CHUNK_SIZE; i++)
        {
            if (buf[i] == '\0')
            {
                strcat(str, buf);
                flag = 1;
                break;
            }
        }
        if (flag == 0)
        {
            strcat(str, buf);
            for (int i = 0; i < 100; i++)
                buf[i] = '\0';
            recv(sockfd, buf, CHUNK_SIZE, 0);
        }
    }
}

int main(int argc, char **argv)
{
    int sockfd1, newsockfd1, sockfd2;
    int clilen;
    struct sockaddr_in cli_addr, serv_addr_s1, serv_addr_s2, lb_addr;
    int load_s1 = 0, load_s2 = 0;

    serv_addr_s1.sin_family = AF_INET;
    inet_aton("127.0.0.1", &serv_addr_s1.sin_addr);
    serv_addr_s1.sin_port = htons(atoi(argv[2]));

    serv_addr_s2.sin_family = AF_INET;
    inet_aton("127.0.0.1", &serv_addr_s2.sin_addr);
    serv_addr_s2.sin_port = htons(atoi(argv[3]));

    lb_addr.sin_family = AF_INET;
    lb_addr.sin_addr.s_addr = INADDR_ANY;
    lb_addr.sin_port = htons(atoi(argv[1]));

    int i;
    char buf[20000];

    if ((sockfd1 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Cannot create socket\n");
        exit(0);
    }

    if (bind(sockfd1, (struct sockaddr *)&lb_addr,
             sizeof(lb_addr)) < 0)
    {
        perror("Unable to bind local address\n");
        exit(0);
    }

    listen(sockfd1, 5);

    while (1)
    {

        if ((sockfd2 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            perror("Unable to create socket\n");
            exit(0);
        }

        if ((connect(sockfd2, (struct sockaddr *)&serv_addr_s1,
                     sizeof(serv_addr_s1))) < 0)
        {
            perror("Unable to connect to server\n");
            exit(0);
        }

        strcpy(buf, "Send load");
        send_str(sockfd2, buf);

        recv_str(sockfd2, buf);
        printf("Load_s1: %s\n", buf);
        load_s1 = atoi(buf);

        close(sockfd2);

        if ((sockfd2 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            perror("Unable to create socket\n");
            exit(0);
        }

        if ((connect(sockfd2, (struct sockaddr *)&serv_addr_s2,
                     sizeof(serv_addr_s2))) < 0)
        {
            perror("Unable to connect to server\n");
            exit(0);
        }

        strcpy(buf, "Send load");
        send_str(sockfd2, buf);

        recv_str(sockfd2, buf);
        printf("Load_s2: %s\n", buf);
        load_s2 = atoi(buf);

        close(sockfd2);

        printf("S1: %d S2: %d\n", load_s1, load_s2);

        struct pollfd fds;
        fds.fd = sockfd1;
        fds.events = POLLIN;
        double t = 5;

        while (poll(&fds, 1, t*1000) > 0)
        {
            if( t <= 0 )break; 
            time_t start_time, end_time;
            // time(&start_time);
            start_time = clock();
            
            // clock_t start_time = clock();

            clilen = sizeof(cli_addr);
            newsockfd1 = accept(sockfd1, (struct sockaddr *)&cli_addr,
                                &clilen);

            if (newsockfd1 < 0)
            {
                perror("Accept error\n");
                exit(0);
            }

            if (fork() == 0)
            {
                close(sockfd1);

                recv_str(newsockfd1, buf);
                printf("%s\n", buf);

                if ((sockfd2 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
                {
                    perror("Unable to create socket\n");
                    exit(0);
                }

                if (load_s1 < load_s2)
                {
                    if ((connect(sockfd2, (struct sockaddr *)&serv_addr_s1,
                                 sizeof(serv_addr_s1))) < 0)
                    {
                        perror("Unable to connect to server\n");
                        exit(0);
                    }

                    printf("S1\n");
                }
                else
                {
                    if ((connect(sockfd2, (struct sockaddr *)&serv_addr_s2,
                                 sizeof(serv_addr_s2))) < 0)
                    {
                        perror("Unable to connect to server\n");
                        exit(0);
                    }

                    printf("S2\n");
                }

                strcpy(buf, "Send time");
                send_str(sockfd2, buf);

                recv_str(sockfd2, buf);
                printf("Date and time: %s\n", buf);

                send(newsockfd1, buf, strlen(buf) + 1, 0);

                
                close(newsockfd1);
                exit(0);
            }
            end_time = clock();
            t -= ((double)(end_time-start_time)/CLOCKS_PER_SEC) * 15000;

            // printf("\nst: %ld et: %ld t: %f\n", start_time, end_time, t);

        }

        // if ((sockfd2 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        // {
        //     perror("Unable to create socket\n");
        //     exit(0);
        // }

        // if ((connect(sockfd2, (struct sockaddr *)&serv_addr_s1,
        //              sizeof(serv_addr_s1))) < 0)
        // {
        //     perror("Unable to connect to server\n");
        //     exit(0);
        // }

        // strcpy(buf, "Send load");
        // send_str(sockfd2, buf);

        // recv_str(sockfd2, buf);
        // printf("Load_s1: %s\n", buf);
        // load_s1 = atoi(buf);

        // close(sockfd2);

        // if ((sockfd2 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        // {
        //     perror("Unable to create socket\n");
        //     exit(0);
        // }

        // if ((connect(sockfd2, (struct sockaddr *)&serv_addr_s2,
        //              sizeof(serv_addr_s2))) < 0)
        // {
        //     perror("Unable to connect to server\n");
        //     exit(0);
        // }

        // strcpy(buf, "Send load");
        // send_str(sockfd2, buf);

        // recv_str(sockfd2, buf);
        // printf("Load_s2: %s\n", buf);
        // load_s2 = atoi(buf);

        // close(sockfd2);

        printf("S1: %d S2: %d\n", load_s1, load_s2);

        close(newsockfd1);
    }
    return 0;
}
