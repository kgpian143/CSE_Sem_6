
/*    THE CLIENT PROCESS */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "mysocket.h"
#define MAX_QUEUE_SIZE 10

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

ssize_t my_send(int Sockfd, const char *buf, size_t len, int flags)
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

ssize_t my_recv(int Sockfd, const char *buf, size_t len, int flags)
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
int main()
{
	int			sockfd ;
	struct sockaddr_in	serv_addr;

	int i;
	char buf[100];

	/* Opening a socket is exactly similar to the server process */
	if ((sockfd = my_socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Unable to create socket\n");
		exit(0);
	}

	/* Recall that we specified INADDR_ANY when we specified the server
	   address in the server. Since the client can run on a different
	   machine, we must specify the IP address of the server. 

	   In this program, we assume that the server is running on the
	   same machine as the client. 127.0.0.1 is a special address
	   for "localhost" (this machine)
	   
	/* IF YOUR SERVER RUNS ON SOME OTHER MACHINE, YOU MUST CHANGE 
           THE IP ADDRESS SPECIFIED BELOW TO THE IP ADDRESS OF THE 
           MACHINE WHERE YOU ARE RUNNING THE SERVER. 
    	*/

	serv_addr.sin_family	= AF_INET;
	inet_aton("127.0.0.1", &serv_addr.sin_addr);
	serv_addr.sin_port	= htons(20000);

	/* With the information specified in serv_addr, the connect()
	   system call establishes a connection with the server process.
	*/
	if ((my_connect(sockfd, (struct sockaddr *) &serv_addr,
						sizeof(serv_addr))) < 0) {
		perror("Unable to connect to server\n");
		exit(0);
	}

	/* After connection, the client can send or receive messages.
	   However, please note that recv() will block when the
	   server is not sending and vice versa. Similarly send() will
	   block when the server is not receiving and vice versa. For
	   non-blocking modes, refer to the online man pages.
	*/
	for(i=0; i < 100; i++) buf[i] = '\0';
	my_recv(sockfd, buf, 100, 0);
	printf("%s\n", buf);

	
	strcpy(buf,"Message from client");
	my_send(sockfd, buf, strlen(buf) + 1, 0);
		
	my_close(sockfd);
	return 0;

}

