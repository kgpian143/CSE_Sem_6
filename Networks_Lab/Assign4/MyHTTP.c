
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

#define CHUNK_SIZE 100

// void send_str( char mess[20000] , int sockfd )
// {
//     int  ind1 = 0, ind2 = 0;
//     char buf[5];
//     while (1)
//     {
//         if (ind1 == 5)
//         {
//             send(sockfd, buf, 5, 0);
//             ind1 = 0;
//         }
//         else if (mess[ind2] == '\0')
//         {
//             buf[ind1++] = '\0';
//             send(sockfd, buf, 5, 0);
//             break;
//         }
            
//         buf[ind1++] = mess[ind2++];
//     }
// }

// void recv_str( char mess[20000] , int id )
// {
//     char buf[10] ; 
//     mess[0] = '\0' ;
//     int k , j , flag =0 ; 
//     for( int i = 0 ; i < 10 ; i++ )buf[i] = '\0' ;
//     while(1)
//     {
//         recv( id , buf  , 5 , 0 ) ;
//         // printf("%s ! %s \n",mess , buf) ;
//         strcat( mess , buf ) ;
//         for( int i = 0 ; i < 5 ; i++ )
//         {
//             if( buf[i] == '\0')
//             {
//                 flag =1 ;
//                 break; 
//             }
//             buf[i] = '\0' ;
//         }
//         if(flag)break;
//     }
// }

void send_str(int sockfd, char str[8196])
{
    int flag = 0, ind1 = 0, ind2 = 0;
    char buf[CHUNK_SIZE+5];
	for (int i = 0; i < 105; i++)
        buf[i] = '\0';
    while (1)
    {
        if (ind1 == CHUNK_SIZE)
        {
            send(sockfd, buf, CHUNK_SIZE, 0);
			// printf("%s", buf);
            ind1 = 0;
        }
        if (str[ind2] == '\0')
            break;
        buf[ind1++] = str[ind2++];
    }
    if (ind1 == CHUNK_SIZE)
    {
        send(sockfd, buf, CHUNK_SIZE, 0);
		// printf("%s", buf);
        ind1 = 0;
        buf[ind1++] = '\0';
        send(sockfd, buf, CHUNK_SIZE, 0);
		// printf("%s", buf);
    }
    else
    {
        buf[ind1++] = '\0';
        send(sockfd, buf, CHUNK_SIZE, 0);
		// printf("%s", buf);
    }
}

int recv_str(int sockfd, char str[1000])
{
    int flag = 0;
    char buf[105];
    for (int i = 0; i < 105; i++)
        buf[i] = '\0';
    recv(sockfd, buf, CHUNK_SIZE, 0);
    strcpy(str, "\0");
    for (int i = 0; i < CHUNK_SIZE; i++)
    {
        if (buf[i] == '\0')
        {
			strcat(str, buf);
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
            for (int i = 0; i < 105; i++)
                buf[i] = '\0';
            recv(sockfd, buf, CHUNK_SIZE, 0);
        }
    }
	return strlen(str) ;
}


int main()
{
	int sockfd, newsockfd;
	int clilen;
	struct sockaddr_in cli_addr, serv_addr;

	int i;
	char buf[100];

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
		printf("Client connected \n");

		if (fork() == 0)
		{

			close(sockfd);

			FILE *fp1;
			fp1 = fopen("AccessLog.txt", "a");
			if (!fp1)
			{
				perror("Error opening file");
				return 1;
			}

			time_t current_time;
			struct tm *local_time;

			current_time = time(NULL);
			local_time = localtime(&current_time);

			char arr[100];
			char str[20];
			sprintf(str, "%02d", local_time->tm_mday);
			strcat(arr, str);
			sprintf(str, "%02d", local_time->tm_mon + 1);
			strcat(arr, str);
			sprintf(str, "%02d", local_time->tm_year % 100);
			strcat(arr, str);
			strcat(arr, " : ");

			sprintf(str, "%02d", local_time->tm_hour);
			strcat(arr, str);
			sprintf(str, "%02d", local_time->tm_min);
			strcat(arr, str);
			sprintf(str, "%02d", local_time->tm_sec);
			strcat(arr, str);
			strcat(arr, " : ");

			char client_ip_str[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &cli_addr.sin_addr, client_ip_str, sizeof(client_ip_str));
			strcat(arr, client_ip_str);
			strcat(arr, " : ");

			int client_port = ntohs(cli_addr.sin_port);
			sprintf(str, "%d", client_port);
			strcat(arr, str);
			strcat(arr, " : ");

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
			printf("If-Modified-Since: %s\n", If_Modified_Since);

			if (strcmp(method, "GET") == 0)
			{
				strcat(arr, "GET");
				strcat(arr, " : ");

				char file_path[1000];
				strcpy(file_path, url);
				printf("file_path : %s\n", file_path);
				send(newsockfd, "HTTP/1.1", 100, 0);

				// printf("Access: %d\n\n", access(file_path, R_OK));
				if (access(file_path, R_OK) != 0)
				{
					// printf("Not readable\n");
					char access_buf[100];
					sprintf(access_buf, "400");
					send(newsockfd, access_buf, 100, 0);
					send(newsockfd, "Permission denied", 100, 0);

					strcat(arr, url);
					strcat(arr, "\n");
					fprintf(fp1, arr);
					fclose(fp1);

					close(newsockfd);
					exit(0);
				}

				// printf("Readable\n");
				FILE *fp = fopen(file_path, "r");
				if (fp != NULL)
				{
					char buffer[100];
					int length;
					printf("Start\n");
					struct stat sb;
					stat(file_path, &sb);
					struct tm *tm2 = gmtime(&sb.st_mtime);

					struct tm tm1;
					memset(&tm1, 0, sizeof(struct tm));
					strptime(If_Modified_Since, "%a, %d %b %Y %H:%M:%S GMT", &tm1);
					time_t t1, t2, diff;
					printf("Year: %d\n", tm1.tm_year + 1900);
					printf("Month: %d\n", tm1.tm_mon + 1);
					printf("Day: %d\n", tm1.tm_mday);
					printf("Hour: %d\n", tm1.tm_hour);
					printf("Minute: %d\n", tm1.tm_min);
					printf("Second: %d\n", tm1.tm_sec);
					t1 = mktime(tm2);
					t2 = mktime(&tm1);

					// char buffer[100];
					printf("t1: %ld \nt2: %ld\n", t1, t2);
					char lmt[100];
					strftime(lmt, 100, "%a, %d %b %Y %H:%M:%S GMT", tm2);
					printf("last modified: %s\n", lmt);

					if (t1 < t2)
					{
						sprintf(buffer, "403");
						send(newsockfd, buffer, 100, 0);
						send(newsockfd, "Bad Request", 100, 0);
					}
					else
					{
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
						send(newsockfd, buffer, 100, 0);

						// Send the file contents
						// printf("FILE CONTENT...\n");
						char read_line[1000];
						while ((length = fread(read_line, 1, 1000, fp)) > 0)
						{
							// printf("%s\n", read_line);
							send(newsockfd, read_line , 1000 , 0 );
							// send_str(newsockfd, read_line);
							// printf("sent: %s\n", read_line);
							for(int i=0; i<1000; i++) read_line[i]='\0';
						}
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
					send(newsockfd, "File Not Found", 100, 0);
				}
			}
			else if (strcmp(method, "PUT") == 0)
			{
				strcat(arr, "PUT");
				strcat(arr, " : ");

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
					if (access(url, W_OK) != 0)
					{
						char access_buf[100];
						sprintf(access_buf, "400");
						send(newsockfd, access_buf, 100, 0);
						send(newsockfd, "Permission denied", 100, 0);
					}
					else
					{

						FILE *fp2 = fopen(file_name, "w");
						char line[1000];
						if (fp2 != NULL)
						{
							char status_code[100];
							strcpy(status_code, "200");
							send(newsockfd, status_code, 100, 0);
							char status_message[100];
							strcpy(status_message, "OK");
							send(newsockfd, status_message, 100, 0);
							int read_size;

							// printf("FILE CONTENT...\n");
							// char read_line[8196];
							for(int i=0; i<1000; i++) line[i]='\0';
							while ((read_size = recv_str(newsockfd, line)) > 0)
							{
								// printf("rcvd: %d\n", read_size);
								// printf("%s\n", line);
								fwrite(line, 1, read_size, fp2);
								for(int i=0; i<1000; i++) line[i]='\0';
							}
							// printf("rcvd: %d\n", read_size);
							fclose(fp2);
						}
						else
						{
							char status_code[100];
							strcpy(status_code, "403");
							send(newsockfd, status_code, 100, 0);
							send(newsockfd, "Bad Request", 100, 0);
						}
						chdir(path);
					}
				}
				else
				{
					char status_code[100];
					strcpy(status_code, "404");
					send(newsockfd, status_code, 100, 0);
					send(newsockfd, "File Not Found", 100, 0);
				}
			}

			strcat(arr, url);
			strcat(arr, "\n");
			fprintf(fp1, arr);
			fclose(fp1);

			close(newsockfd);
			exit(0);
		}

		close(newsockfd);
	}
	return 0;
}
