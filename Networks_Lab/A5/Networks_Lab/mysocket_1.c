#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SOCK_MyTCP 1



int my_socket(int domain, int type, int protocol)
{
   int sockfd = socket(domain, type, protocol);
   if (sockfd == -1)
   {
      perror("socket");
      return -1;
   }
   return sockfd;
};
int my_bind(int fd, const struct sockaddr *addr, socklen_t len)
{
    int val = bind(fd, addr, len);
    
    return val;
};
int my_listen(int fd, int n)
{
    int val = listen(fd, n);
    return val;
};
int my_accept(int fd, struct sockaddr * addr, socklen_t * addr_len)
{
    int newsockfd = accept(fd, addr, addr_len);
    return newsockfd;
};
int my_connect(int fd, const struct sockaddr *addr, socklen_t len)
{
    int con_val = connect(fd, addr, len);
    return con_val;
};
int my_send(int fd, const void * buf, size_t n, int flags)
{
    int send_val = send(fd, buf, n, flags);
    return send_val;
};
int my_close(int fd)
{
    
};

int main(int argc, char *argv[])
{   
    // printf("%d\n", SOCK_STREAM);

    return 0;
}