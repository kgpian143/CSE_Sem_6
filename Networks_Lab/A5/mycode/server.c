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
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
// #include "mysocket.h"
#define MAX_QUEUE_SIZE 10
#define   SOCK_MyTCP  1234 

typedef struct
{
    char *items[MAX_QUEUE_SIZE];
    int front;
    int rear;
} Queue;

Queue *createQueue()
{
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    queue->front = 0;
    queue->rear = -1;
    return queue;
}

void destroyQueue(Queue *queue)
{
    free(queue);
}

int isEmpty(Queue *queue)
{
    return (queue->rear < queue->front);
}

int isFull(Queue *queue)
{
    return (queue->rear == MAX_QUEUE_SIZE - 1);
}

void enqueue(Queue *queue, char *item)
{
    if (isFull(queue))
    {
        printf("Error: Queue is full\n");
        exit(1);
    }
    queue->rear++;
    queue->items[queue->rear] = (char *)malloc(strlen(item) + 1);
    strcpy(queue->items[queue->rear], item);
}

char *dequeue(Queue *queue)
{
    if (isEmpty(queue))
    {
        printf("Error: Queue is empty\n");
        exit(1);
    }
    char *item = queue->items[queue->front];
    queue->front++;
    return item;
}

Queue *Send_Message, *Received_Message;

pthread_t R, S;

pthread_mutex_t Rq_lock, Sq_lock;
pthread_cond_t Rq_cond, Sq_cond;

void *func_R(void *arg)
{
    while (1)
    {
        int sockfd = atoi((char *)arg);
        char buf[1000];
        char message[5005];
        while (1)
        {
            int n = recv(sockfd, buf, 1000, 0);
            if (n == 0)
                break;
            strcat(message, buf);
            for (int i = 0; i < 1000; i++)
                buf[i] = '\0';
        }

        // Critical Section Starts

        pthread_mutex_lock(&Rq_lock);

        while (isFull(Received_Message))
            pthread_cond_wait(&Rq_cond, &Rq_lock);
        enqueue(Received_Message, message);

        pthread_cond_signal(&Rq_cond);
        pthread_mutex_unlock(&Rq_lock);

        // Critical Section Ends
    }
    pthread_exit(NULL);
}

void *func_S(void *arg)
{
    while (1)
    {
        int sockfd = atoi((char *)arg);
        char buf[1000];
        char message[5005];


        // Critical Section Starts

        pthread_mutex_lock(&Sq_lock);

        while (isEmpty(Send_Message) != 1)
        {
            pthread_cond_wait(&Sq_cond, &Sq_lock);
        }
        strcpy(message, dequeue(Send_Message));

        pthread_cond_signal(&Sq_cond);
        pthread_mutex_unlock(&Sq_lock);

        // Critical Section Ends

        int j = 0;
        for (int i = 0; i < 5000; i++)
        {
            if (j == 999)
            {
                send(sockfd, buf, 1000, 0);
                for (int k = 0; k < 1000; k++)
                    buf[k] = '\0';
            }
            buf[j++] = message[i];
        }
        send(sockfd, buf, 1000, 0);
        for (int k = 0; k < 1000; k++)
            buf[k] = '\0';
        send(sockfd, buf, strlen(buf), 0);
        // sleep(5);
    }
    pthread_exit(NULL);
}
int my_socket(int family, int type, int protocol)
{
    int fd = socket(family, SOCK_STREAM, protocol);

    char sockfd[10]; 

    sprintf( sockfd ,"%d", fd ) ;
    // cond inits
    pthread_cond_init(&Rq_cond, NULL);
    pthread_cond_init(&Sq_cond, NULL);

    // mutex inits
    pthread_mutex_init(&Rq_lock, NULL);
    pthread_mutex_init(&Sq_lock, NULL);

    Send_Message = createQueue();
    Received_Message = createQueue();
    pthread_create(&R, NULL, func_R, (void *)sockfd);
    pthread_create(&S, NULL, func_S, (void *)sockfd);
    return fd;
}

int my_bind(int Sockfd, struct sockaddr *addr, socklen_t addrlen)
{
    return bind(Sockfd, addr,
                addrlen);
}

int my_listen(int Sockfd, int clients)
{
    return listen(Sockfd, clients);
}

int my_accept(int Sockfd, struct sockaddr *addr, socklen_t addrlen)
{
    return connect(Sockfd, addr, addrlen);
}

ssize_t my_send(int Sockfd,  char *buf, size_t len, int flags)
{

    // Critical Section Starts

    pthread_mutex_lock(&Sq_lock);
    while (isFull(Send_Message))
    {
        pthread_cond_wait(&Sq_cond, &Sq_lock);
    }
    enqueue(Send_Message, buf);

    pthread_cond_signal(&Sq_cond);
    pthread_mutex_unlock(&Sq_lock);

    // Critical Section Ends
    return strlen(buf);
}

ssize_t my_recv(int Sockfd,  char *buf, size_t len, int flags)
{
    // Critical Section Starts
    
    pthread_mutex_lock(&Rq_lock);
    while (isEmpty(Received_Message))
    {  
        pthread_cond_wait(&Rq_cond, &Rq_lock); 
    }
    strcpy(buf, dequeue(Received_Message));
    pthread_cond_signal(&Rq_cond);
    pthread_mutex_unlock(&Rq_lock);

    // Critical Section Ends
    return strlen(buf);
}

int my_close(int Sockfd)
{
    pthread_cancel(R);
    pthread_cancel(S);
    destroyQueue(Send_Message);
    destroyQueue(Received_Message);
    close(Sockfd);
}
			/* THE SERVER PROCESS */

int main()
{
	int			sockfd, newsockfd ; /* Socket descriptors */
	int			clilen;
	struct sockaddr_in	cli_addr, serv_addr;

	int i;
	char buf[100];		/* We will use this buffer for communication */

	/* The following system call opens a socket. The first parameter
	   indicates the family of the protocol to be followed. For internet
	   protocols we use AF_INET. For TCP sockets the second parameter
	   is SOCK_STREAM. The third parameter is set to 0 for user
	   applications.
	*/
	if ((sockfd = my_socket(AF_INET, SOCK_MyTCP , 0)) < 0) {
		perror("Cannot create socket\n");
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
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= INADDR_ANY;
	serv_addr.sin_port		= htons(20000);

	/* With the information provided in serv_addr, we associate the server
	   with its port using the bind() system call. 
	*/
	if (my_bind(sockfd, (struct sockaddr *) &serv_addr,
					sizeof(serv_addr)) < 0) {
		perror("Unable to bind local address\n");
		exit(0);
	}

	my_listen(sockfd, 5); /* This specifies that up to 5 concurrent client
			      requests will be queued up while the system is
			      executing the "accept" system call below.
			   */

	/* In this program we are illustrating an iterative server -- one
	   which handles client connections one by one.i.e., no concurrency.
	   The accept() system call returns a new socket descriptor
	   which is used for communication with the server. After the
	   communication is over, the process comes back to wait again on
	   the original socket descriptor.
	*/
	while (1) {

		/* The accept() system call accepts a client connection.
		   It blocks the server until a client request comes.

		   The accept() system call fills up the client's details
		   in a struct sockaddr which is passed as a parameter.
		   The length of the structure is noted in clilen. Note
		   that the new socket descriptor returned by the accept()
		   system call is stored in "newsockfd".
		*/
		clilen = sizeof(cli_addr);
		newsockfd = my_accept(sockfd, (struct sockaddr *) &cli_addr,
					&clilen) ;

		if (newsockfd < 0) {
			perror("Accept error\n");
			exit(0);
		}


		/* We initialize the buffer, copy the message to it,
			and send the message to the client. 
		*/
		
		strcpy(buf,"Message from server");
		my_send(newsockfd, buf, strlen(buf) + 1, 0);

		/* We now receive a message from the client. For this example
  		   we make an assumption that the entire message sent from the
  		   client will come together. In general, this need not be true
		   for TCP sockets (unlike UDPi sockets), and this program may not
		   always work (for this example, the chance is very low as the 
		   message is very short. But in general, there has to be some
	   	   mechanism for the receiving side to know when the entire message
		  is received. Look up the return value of recv() to see how you
		  can do this.
		*/ 
		my_recv(newsockfd, buf, 100, 0);
		printf("%s\n", buf);

		my_close(newsockfd);
	}
	return 0;
}
			

