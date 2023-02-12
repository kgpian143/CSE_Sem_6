// NETWORKS LAB (Assignent: 01, Qn: 01, CLIENT PROCESS)
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

int main()
{
    int sockfd;
    struct sockaddr_in serv_addr;

    // int i;
    char buf[100];

    // printf("\nConnected to server\n");

    char method[100];

    while (1)
    {
        printf("\nMyOwnBrowser> ");
        scanf("%s", method);
        // printf("%s\n", method);
        if (strcmp(method, "QUIT") == 0)
        {
            break;
        }
        char cmd[100];
        gets(cmd);
        // printf("%s\n", cmd);
        // send(sockfd, cmd, 100, 0);
        int index;
        for (int i = 0; i < strlen(cmd) - 1; i++)
        {
            // printf("%c ", cmd[i]);
            if (cmd[i] == '/' && cmd[i + 1] == '/')
            {
                index = i + 2;
                break;
            }
        }
        // printf("\n\n");
        char url[100], ip[100], port[20];
        char file_name[100];
        int url_index = 0, ip_index = 0, flag1 = 0 , flag3 = 0 ;
        for (int i = 0; i < 100; i++)
            ip[i] = '\0';
        for (int i = index; i < strlen(cmd); i++)
        {
            if (cmd[i] == '/')
                flag1++;
            if (cmd[i] == ' ')
            {
                for (int j = i + 1; j < strlen(cmd); j++)
                    file_name[j - i - 1] = cmd[j];
                break;
            }

            if (cmd[i] == ':')
            {
                // int j = i ; 
                for (int j = i + 1; j < strlen(cmd); j++)
                    port[j - i - 1] = cmd[j] ;
                flag3++ ; 
                // break;
            }
            if (flag1 == 0)
                ip[ip_index++] = cmd[i];
            else if( flag3 == 0 )
                url[url_index++] = cmd[i];
            // url_index++;
        }
        url[url_index] = '\0';
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            perror("Unable to create socket\n");
            exit(0);
        }
        if (strlen(port) == 0)
            strcpy(port, "80");
        printf("%s %s\n", ip, port);
        serv_addr.sin_family = AF_INET;
        inet_aton(ip, &serv_addr.sin_addr);
        serv_addr.sin_port = htons(atoi(port));

        if ((connect(sockfd, (struct sockaddr *)&serv_addr,
                     sizeof(serv_addr))) < 0)
        {
            perror("Unable to connect to server\n");
            exit(0);
        }
        // printf("%s\n", url);
        send(sockfd, method, 100, 0);
        send(sockfd, url, 100, 0);
        char protocol_version[100];
        strcpy(protocol_version, "HTTP/1.1");
        send(sockfd, protocol_version, 100, 0);

        int flag = 0, last = -1;
        char Host[100];
        strcpy(Host, "Host: ");
        int ind = strlen(Host);
        for (int i = index; i < strlen(cmd); i++)
        {
            if (cmd[i] == '/')
            {
                flag++;
                last = i;
            }
            else if (flag == 0)
            {
                Host[ind] = cmd[i];
                ind++;
            }
        }
        Host[ind] = '\0';
        // printf("%s\n", Host);
        send(sockfd, Host, 100, 0);

        char Connection[100];
        strcpy(Connection, "Connection: close");
        // printf("%s\n", Connection);
        send(sockfd, Connection, 100, 0);

        char Date[100];
        strcpy(Date, "Date: ");
        time_t rawtime, modified_time;
        struct tm *timeinfo;
        char buffer[80];

        time(&rawtime);
        modified_time = rawtime - (2 * 24 * 60 * 60);
        timeinfo = gmtime(&rawtime);

        strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
        // printf("%s\n", buffer);
        strcat(Date, buffer);
        // printf("%s\n", Date);
        send(sockfd, Date, 100, 0);
        char content_type[100];
        strcpy(content_type, "Content Type : ");
        char Accept[100];
        strcpy(Accept, "Accept: ");
        int ind2 = 8, flag2 = 0;
        for (int i = last + 1; i < strlen(cmd); i++)
        {
            if (cmd[i] == '.')
            {
                flag2++;
                // printf("diff: %d\n", strlen(cmd)-i);
                if ((strlen(cmd) - i == 4))
                {
                    if (cmd[i + 1] == 'p' && cmd[i + 2] == 'd' && cmd[i + 3] == 'f')
                    {
                        strcat(Accept, "application/pdf");
                        strcat(content_type, "application/pdf");
                    }
                    else if (cmd[i + 1] == 'j' && cmd[i + 2] == 'p' && cmd[i + 3] == 'g')
                    {
                        strcat(Accept, "image/jpeg");
                        strcat(content_type, "image/jpeg");
                    }
                    else
                    {
                        strcat(Accept, "text/*");
                        strcat(content_type, "text/*");
                    }
                }
                else if ((strlen(cmd) - i == 5))
                {
                    if (cmd[i + 1] == 'h' && cmd[i + 2] == 't' && cmd[i + 3] == 'm' && cmd[i + 4] == 'l')
                    {
                        strcat(Accept, "text/html");
                        strcat(content_type, "text/html");
                    }
                    else
                    {
                        strcat(Accept, "text/*");
                        strcat(content_type, "text/*");
                    }
                }
                else if ((strlen(cmd) - i > 5))
                {
                    if (cmd[i + 1] == 'p' && cmd[i + 2] == 'd' && cmd[i + 3] == 'f' && cmd[i + 4] == ':')
                    {
                        strcat(Accept, "application/pdf");
                        strcat(content_type, "application/pdf");
                    }
                    else if (cmd[i + 1] == 'j' && cmd[i + 2] == 'p' && cmd[i + 3] == 'g' && cmd[i + 4] == ':')
                    {
                        strcat(Accept, "image/jpeg");
                        strcat(content_type, "image/jpeg");
                    }
                    else if (cmd[i + 1] == 'h' && cmd[i + 2] == 't' && cmd[i + 3] == 'm' && cmd[i + 4] == 'l' && cmd[i + 5] == ':')
                    {
                        strcat(Accept, "text/html");
                        strcat(content_type, "text/html");
                    }
                    else
                    {
                        strcat(Accept, "text/*");
                        strcat(content_type, "text/*");
                    }
                }
                else
                {
                    strcat(Accept, "text/*");
                    strcpy(content_type, "text/*");
                }

                break;
            }
        }
        // printf("%s\n", Accept);
        send(sockfd, Accept, 100, 0);

        char Accept_Language[100];
        strcpy(Accept_Language, "Accept-Language: en-us");
        // printf("%s\n", Accept_Language);
        send(sockfd, Accept_Language, 100, 0);

        char If_Modified_Since[100];
        strcpy(If_Modified_Since, "If-Modified-Since: ");
        timeinfo = gmtime(&modified_time);
        char buff1[80];
        strftime(buff1, 80, "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
        strcat(If_Modified_Since, buff1);
        // printf("%s\n", If_Modified_Since);
        send(sockfd, If_Modified_Since, 100, 0);

        int k = 0;
        
        if (strcmp(method, "GET") == 0)
        {
            for (int i = 0; url[i] != '\0'; i++)
            {
                if (url[i] == '/')
                {
                    k = 0;
                    continue;
                }
                file_name[k++] = url[i];
            }
            file_name[k] = '\0';
            printf("file name : %s\n", file_name);
            char version[100];
            char status_code[100];
            char status_message[100];
            recv(sockfd, version, 100, 0);
            printf("version %s\n", version);
            recv(sockfd, status_code, 100, 0);
            printf("status code : %s\n", status_code);
            if (strcmp(status_code, "200") == 0)
            {
                recv(sockfd, status_message, 100, 0);
                printf("Status message : %s\n",status_message) ;
                char expire_time[100];
                recv(sockfd, expire_time, 100, 0);
                printf("Expires time : %s\n", expire_time);
                char content_lang[100];
                recv(sockfd, content_lang, 100, 0);
                printf("%s\n", content_lang);
                char buff1[100];
                recv(sockfd, buff1, 100, 0);
                char server_reply[8196];
                FILE *fp = fopen(file_name, "w");
                if (fp != NULL)
                {
                    int read_size;
                    while ((read_size = recv(sockfd, server_reply, 8196, 0)) > 0)
                    {
                        fwrite(server_reply, 1, read_size, fp);
                    }
                    fclose(fp);
                }
            }
            else if( strcmp(status_code , "404") == 0 )
            {
                printf("Request is not found\n") ;
            }

            else if( strcmp( status_code , "400" ) == 0 )
            {
                printf("Permission denied for the request\n") ;
            }
        }
        else if (strcmp(method, "PUT") == 0)
        {
            char content_language[100];
            strcpy(content_language, "Content Lang : en-us");
            send(sockfd, content_language, 100, 0);
            FILE *fp = fopen(file_name, "r");
            if (fseek(fp, 0, SEEK_END) != 0)
            {
                perror("fseek");
                fclose(fp);
                return 1;
            }
            fclose(fp);
            long long int length = ftell(fp);
            char content_length[100];
            sprintf(content_length, "%lld", length);
            send(sockfd, content_length, 100, 0);

            send(sockfd, content_type, 100, 0);

            send(sockfd, file_name, 100, 0);
            // char status_code[100] ;
            char version[100];
            char status_code[100];
            char status_message[100];
            recv(sockfd, version, 100, 0);
            printf("version %s\n", version);
            recv( sockfd , status_code , 100 , 0 ) ; 
            if(  strcmp( status_code , "200") == 0  )
            {
                recv(sockfd, status_message, 100, 0);
                printf("Status message : %s\n",status_message) ;
                fp = fopen(file_name, "r");
                char line_buff[8196];
                int line_len;
                while ((line_len = fread(line_buff, 1, 8196, fp)) > 0)
                {
                    send(sockfd, line_buff, line_len, 0);
                }
            }
            else if( strcmp(status_code , "404") == 0)
            {
               printf("The directory you want to access is not found\n" ) ;
            }
            else if( strcmp( status_code , "400" ) == 0 )
            {
                printf("Permission denied for the request\n") ;
            }

        }

        close(sockfd);
    }

    return 0;
}