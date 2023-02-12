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

/* THE SERVER PROCESS */

int k;
int isOperator(char ch)
{
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%')
        return 1;
    else
        return 0;
}
double tokenization(char *str)
{
    while( str[k] == ' ')k++ ;
    double val = 0;
    int flag = 0;
    double dec = 0.1;
    while (1)
    {
        if (str[k] == '\0')
            return val;
        else if (isOperator(str[k]) || str[k] == '(' || str[k] == ')')
            return val;
        else if (str[k] == ' ')
            return val;
        if (flag)
        {
            val += dec * (str[k] - '0');
            dec /= 10;
        }
        else if (str[k] == '.')
        {
            flag = 1;
        }
        else
        {
            val = val * 10 + (str[k] - '0');
        }
        k++;
    }
    return val;
}

double calc(double a, double b, char c)
{
    if (c == '+')
        return (a + b);
    else if (c == '-')
        return (a - b);
    else if (c == '*')
        return (a * b);
    else if (c == '/')
        return (a / b);
    else
        return 0;
}

double evaluate(char *str)
{
    k = 0 ;
    int flag = 0;
    char op;
    double ans1 = 0, ans2 = 0;
    if (str[k] == '(')
    {
        op = '+';
        k++;
        flag = 1;
        ans2 = tokenization(str);
    }
    else
    {
        ans1 = tokenization(str);
    }
    while (str[k] != '\0')
    {
        if (str[k] == ' ')
        {
            k++;
            continue;
        }
        if (flag)
        {
            if (isOperator(str[k]))
            {
                char op1 = str[k];
                k++;
                double token = tokenization(str);
                ans2 = calc(ans2, token, op1);
            }
            if (str[k] == ')')
            {
                flag = 0;
                ans1 = calc(ans1, ans2, op);
                ans2 = 0;
                k++;
            }
        }
        else
        {
            if (isOperator(str[k]) && str[k + 1] != '(')
            {
                char op = str[k];
                k++;
                double token = tokenization(str);
                ans1 = calc(ans1, token, op);
            }
            if (str[k] == '\0')
                break;
            if (str[k] == '(')
            {
                flag = 1;
                op = str[k - 1];
                k++;
                ans2 = tokenization(str);
            }
            if (isOperator(str[k]) && str[k + 1] == '(')
                k++;
            // if(str[i] == ')')i++ ;
        }
    }
    return ans1;
} 
int main()
{
    int sockfd, newsockfd; /* Socket descriptors */
    int clilen;
    struct sockaddr_in cli_addr, serv_addr;

    int i;
    char buf[10]; /* We will use this buffer for communication */

    /* The following system call opens a socket. The first parameter
       indicates the family of the protocol to be followed. For internet
       protocols we use AF_INET. For TCP sockets the second parameter
       is SOCK_STREAM. The third parameter is set to 0 for user
       applications.
    */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
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
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(20000);

    /* With the information provided in serv_addr, we associate the server
       with its port using the bind() system call.
    */
    if (bind(sockfd, (struct sockaddr *)&serv_addr,
             sizeof(serv_addr)) < 0)
    {
        perror("Unable to bind local address\n");
        exit(0);
    }

    listen(sockfd, 5); /* This specifies that up to 5 concurrent client
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
            perror("Accept error\n");
            exit(0);
        }

        /* We initialize the buffer, copy the message to it,
            and send the message to the client.
        */

        // strcpy(buf,"Message from server");
        // send(newsockfd, buf, strlen(buf) + 1, 0);

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
        // while (recv(newsockfd, buf, 10, 0) > 0)
        // {
        //     printf("%s",buf) ;
        //     char *str;
        //     int flag = 0, str_size = 0, str_cap = 10;
        //     str = malloc(10);
        //     for (int i = 0; i < 10; i++)
        //     {
        //         if (buf[i] == '\0')
        //         {
        //             str[str_size++] = buf[i];
        //             flag == 1;
        //             break;
        //         }
        //         str[str_size++] = buf[i];
        //     }

        //     while (flag == 0)
        //     {
        //         // for (int i = 0; i < 10; i++)
        //         //     buf[i] = '\0';
        //         if (recv(newsockfd, buf, 10, 0) > 0)
        //         {
        //             printf("%s",buf);
        //             str = realloc( str , str_cap + 10 );
        //             str_cap += 10 ;
        //             for (int i = 0; i < 10; i++)
        //             {
        //                 if (buf[i] == '\0')
        //                 {
        //                     str[str_size++] = buf[i];
        //                     flag == 1;
        //                     break;
        //                 }
        //                 str[str_size++] = buf[i];
        //             }
        //         }
        //     }
        //     i = 0;
        //     char str_ans[100];
        //     printf("%s\n", str);
            // double ans = evaluate(str);
            // sprintf(str_ans, "%f", ans);
            // char buff_send[50] ;
            // strcpy(buff_send, str_ans);
            // send(newsockfd, buff_send, strlen(buff_send) + 1, 0);
            // printf("Expression evalutes : %s\n",buf) ;
            // for (int i = 0; i < 10; i++)
            //     buff_send[i] = '\0';
        // }
        char *str;

        while (recv(newsockfd, buf, 11 , 0) >= 0)
        {
            // printf("%s\n", buf);
            char *str;
            int flag = 0, str_size = 0, str_cap = 10;
            str = malloc(11);
            
            while ((flag == 0) && recv(newsockfd, buf, 10, 0) >= 0)
            {
                printf("%s \n",buf) ;
            }
            // printf("Expression from server : %s \n", str);
            char str_ans[50] ;
            double ans = 0 ;
            // double ans = evaluate(str);
            // printf("%lf \n",ans) ;
            sprintf(str_ans, "%f", ans);
            char buff_send[50] ;
            strcpy(buff_send, str_ans);
            send(newsockfd, buff_send, strlen(buff_send) + 1, 0);
            // printf("Expression evalutes : %s\n",buff_send) ;
            for (int i = 0; i < 50; i++)
                buff_send[i] = '\0';
        }
        close(newsockfd);
    }
    return 0;
}
