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

#define CHUNK_SIZE 10

void send_str( int sockfd , char str[20000])
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

void recv_str( int sockfd , char str[20000])
{
    int flag = 0 ; 
    char buf[100] ; 
    for(int i=0; i<100; i++) buf[i]='\0';
    recv( sockfd , buf , CHUNK_SIZE , 0 ) ; 
    strcpy( str , buf ) ;
    for (int i = 0; i < CHUNK_SIZE; i++)
    {
        if (buf[i] == '\0')
        {
            flag = 1;
            break;
        }
    }
    while( flag == 0 )
    {
        for( int i = 0 ; i < CHUNK_SIZE ; i++  )
        {
            if( buf[i] == '\0')
            {
                strcat( str , buf ) ; 
                flag = 1 ; 
                break;
            }
        }
        if( flag == 0 )
        {
            strcat(str , buf ) ;
            for( int i = 0 ; i < 100 ; i++ )buf[i] = '\0' ;
            recv( sockfd , buf , CHUNK_SIZE , 0 ) ;
        }
    }
}

int main(int argc, char **argv)
{
	int			sockfd ;
	struct sockaddr_in	serv_addr;

    serv_addr.sin_family	= AF_INET;
	inet_aton("127.0.0.1", &serv_addr.sin_addr);
	serv_addr.sin_port	= htons(atoi(argv[1]));

	int i;
	char buf[100];

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Unable to create socket\n");
		exit(0);
	}

	if ((connect(sockfd, (struct sockaddr *) &serv_addr,
						sizeof(serv_addr))) < 0) {
		perror("Unable to connect to server\n");
		exit(0);
	}

	// printf("\nConnected to server\n");

    // strcpy(buf,"Send load");
	// send(sockfd, buf, strlen(buf) + 1, 0);

    // for(i=0; i < 100; i++) buf[i] = '\0';
	// recv(sockfd, buf, 100, 0);
	// printf("Load: %s\n", buf);

    // close(sockfd);




    // if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	// 	perror("Unable to create socket\n");
	// 	exit(0);
	// }

    // if ((connect(sockfd, (struct sockaddr *) &serv_addr,
	// 					sizeof(serv_addr))) < 0) {
	// 	perror("Unable to connect to server\n");
	// 	exit(0);
	// }

	// printf("\nConnected to server again\n");

    strcpy(buf,"Send time");
	send_str(sockfd, buf);

	recv_str(sockfd, buf);
	printf("Date and time: %s\n", buf);


	// strcpy(buf,"Message from client");
	// send(sockfd, buf, strlen(buf) + 1, 0);
		
	close(sockfd);
	return 0;

}

