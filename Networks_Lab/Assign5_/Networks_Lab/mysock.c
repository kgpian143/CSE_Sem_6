#include "mysock.h"

// structure to hold send and received messages

// structure to hold received messages
MessageTable *send_table=NULL;
MessageTable *recv_table = NULL;
pthread_t R_thread, S_thread;

int mysockfd = -1;
int is_connect = 0;
int recv_sockfd = -1;
int send_sockfd = -1;
// R thread for receiving messages

// error in retrieving message
void *R(void *arg)
{

            printf("6\n");
            // sleep(8);
    while (1)
    {
        int sockfd = mysockfd;
        if(recv_sockfd == -1)continue;
        char buffer[BUFFER_SIZE];
        int bytes_received;
        if (is_connect == 0)
            continue;
        bytes_received = recv(recv_sockfd, buffer, BUFFER_SIZE, 0);
        
        printf("p:%d\n", bytes_received);
        if (bytes_received < 0)
        {
            perror("Error receiving message");
            exit(EXIT_FAILURE);
            // continue;
        }
        else if (bytes_received == 0)
        {
            printf("Connection closed by remote host.\n");
            exit(EXIT_SUCCESS);
        }
        else
        {
            pthread_mutex_lock(&recv_table->mutex);
            while (recv_table->count == BUFFER_SIZE)
            {
                pthread_cond_wait(&recv_table->sig, &recv_table->mutex);
            }
            strcpy(recv_table->messages[recv_table->tail], buffer);
            recv_table->tail = (recv_table->tail + 1) % BUFFER_SIZE;
            recv_table->count++;
            pthread_cond_signal(&recv_table->sig);
            pthread_mutex_unlock(&recv_table->mutex);
        }
    }
    pthread_exit(NULL);
}

// S thread for sending messages
// segmentation fault in *s
void *S(void *arg)
{
            // sleep(5);

    while (1)
    {
        int sockfd = mysockfd;
        if(send_sockfd == -1)continue;

        char buffer[5000];
        int bytes_sent;
        
        printf("send table size : %d",send_table->count);
        pthread_mutex_lock(&send_table->mutex);
        while (send_table->count == 0)
        {
            pthread_cond_wait(&send_table->sig, &send_table->mutex);
        }
        strcpy(buffer, send_table->messages[send_table->head]);
        send_table->head = (send_table->head + 1) % BUFFER_SIZE;
        send_table->count--;
        pthread_cond_signal(&send_table->sig);
        pthread_mutex_unlock(&send_table->mutex);

        bytes_sent = send(send_sockfd, buffer, strlen(buffer) + 1, 0);
        printf("send : %d\n", bytes_sent);
        if (bytes_sent < 0)
        {
            perror("Error sending message");
            exit(EXIT_FAILURE);
        }
    }
    pthread_exit(NULL);
}

// my_socket function
int my_socket(int domain, int type, int protocol)
{
    int sockfd = socket(domain, SOCK_STREAM, protocol);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }
    mysockfd = sockfd;
    // allocate space for send and receive message tables
    send_table = (MessageTable *)malloc(sizeof(MessageTable));
    send_table->head = 0;
    send_table->tail = 0;
    send_table->count = 0;
    // printf("%d\n",send_table->count);

    pthread_mutex_init(&send_table->mutex, NULL);
    pthread_cond_init(&send_table->sig, NULL);
    pthread_cond_init(&send_table->sig, NULL);
    recv_table = (MessageTable *)malloc(sizeof(MessageTable));

    recv_table->head = 0;
    recv_table->tail = 0;
    recv_table->count = 0;
    pthread_mutex_init(&recv_table->mutex, NULL);
    pthread_cond_init(&recv_table->sig, NULL);
    pthread_cond_init(&recv_table->sig, NULL);

    if (pthread_create(&R_thread, NULL, R, NULL) != 0)
    {
        perror("Error creating R thread");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&S_thread, NULL, S, NULL) != 0)
    {
        perror("Error creating S thread");
        exit(EXIT_FAILURE);
    }
    printf("8\n");
    // join R and S threads

    return sockfd;
}

// my_bind function
int my_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    int result = bind(sockfd, addr, addrlen);
    if (result < 0)
    {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }
    return result;
}

// my_listen function
int my_listen(int sockfd, int backlog)
{
    int result = listen(sockfd, backlog);
    if (result < 0)
    {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }
    return result;
}

// my_accept function
int my_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    int result = accept(sockfd, addr, addrlen);
    // mysockfd = 1;
    is_connect = 1;
    if (result < 0)
    {
        perror("Error accepting connection");
        exit(EXIT_FAILURE);
    }
    return result;
}

// my_connect function
int my_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    int connect_val = connect(sockfd, addr, addrlen);
    if (connect_val < 0)
    {
        perror("Error accepting connection");
        exit(EXIT_FAILURE);
    }
    return connect_val;
}

int my_send(int sockfd, const void *buf, size_t len, int flags)
{
    send_sockfd=sockfd;
    printf("9\n");

    pthread_mutex_lock(&send_table->mutex);
    if (send_table->count == MAX_SIZE)
    {
        pthread_cond_wait(&send_table->sig, &send_table->mutex);
    }

    send_table->messages[send_table->tail] = (char *)malloc(strlen((char *)buf) + 1);
    printf("mess sent : %s \n",(char *)buf ) ;
    memcpy(send_table->messages[send_table->tail], buf, strlen((char *)buf) + 1);
    printf("mess inserted : %s \n",send_table->messages[send_table->tail] ) ;
    send_table->count++;
    send_table->tail = (send_table->tail + 1) % MAX_SIZE;
    pthread_cond_signal(&send_table->sig);
    pthread_mutex_unlock(&send_table->mutex);
    return len;
}

// my_recv function
int my_recv(int sockfd, void *buf, size_t len, int flags)
{
    recv_sockfd=sockfd;
    
    // receive message from the recv_table
    pthread_mutex_lock(&recv_table->mutex);
    // printf("My_recv has acquired mutex lock");
    while (recv_table->count == 0)
    {
        printf("19\n");
        pthread_cond_wait(&recv_table->sig, &recv_table->mutex);
        printf("20\n");
    }
    int message_len = strlen(recv_table->messages[recv_table->head]) + 1;
    
    memcpy(buf, recv_table->messages[recv_table->head], message_len);
    recv_table->head = (recv_table->head + 1) % BUFFER_SIZE;
    recv_table->count--;
    pthread_cond_signal(&recv_table->sig);
    pthread_mutex_unlock(&recv_table->mutex);

    return message_len;
}

int my_close(int sockfd)
{

    // free memory allocated for message tables
    free(send_table);
    free(recv_table);

    // destroy mutexes and condition variables
    pthread_mutex_destroy(&send_table->mutex);
    pthread_cond_destroy(&send_table->sig);
    pthread_cond_destroy(&send_table->sig);
    pthread_mutex_destroy(&recv_table->mutex);
    pthread_cond_destroy(&recv_table->sig);
    pthread_cond_destroy(&recv_table->sig);

    // cancel threads
    pthread_cancel(S_thread);
    pthread_cancel(R_thread);

    // close the socket
    int result = close(sockfd);
    return result;
}