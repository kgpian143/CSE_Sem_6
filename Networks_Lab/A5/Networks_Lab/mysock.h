#ifndef MYSOCK_H
#define MYSOCK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define BUFFER_SIZE 1000
#define SOCK_MyTCP 1
#define MAX_SIZE 10

typedef struct
{
    char *messages[MAX_SIZE];
    int head;
    int tail;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t sig ;
} MessageTable;

int my_socket(int domain, int type, int protocol);
int my_bind(int fd, const struct sockaddr *addr, socklen_t len);
int my_listen(int fd, int n);
int my_accept(int fd, struct sockaddr * addr, socklen_t * addr_len);
int my_connect(int fd, const struct sockaddr *addr, socklen_t len);
int my_send(int fd, const void * buf, size_t n, int flags);
int my_recv(int sockfd, void *buf, size_t len, int flags);
int my_close(int fd);


#endif