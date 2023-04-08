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

int i;
int isOperator(char ch)
{
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
        return 1;
    else
        return 0;
}
double tokenization(char *str)
{
    while (str[i] == ' ')
        i++;
    double val = 0;
    int flag = 0;
    double dec = 0.1;
    while (1)
    {
        if (str[i] == '\0')
            return val;
        else if (isOperator(str[i]) || str[i] == '(' || str[i] == ')')
            return val;
        else if (str[i] == ' ')
            return val;
        if (flag)
        {
            val += dec * (str[i] - '0');
            dec /= 10;
        }
        else if (str[i] == '.')
        {
            flag = 1;
        }
        else
        {
            val = val * 10 + (str[i] - '0');
        }
        i++;
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
    i = 0;
    int flag = 0;
    char op;
    double ans1 = 0, ans2 = 0;
    if (str[i] == '(')
    {
        op = '+';
        i++;
        flag = 1;
        ans2 = tokenization(str);
    }
    else
    {
        ans1 = tokenization(str);
    }
    while (str[i] != '\0')
    {
        if (str[i] == ' ')
        {
            i++;
            continue;
        }
        if (flag)
        {
            if (isOperator(str[i]))
            {
                char op1 = str[i];
                i++;
                double token = tokenization(str);
                ans2 = calc(ans2, token, op1);
            }
            if (str[i] == ')')
            {
                flag = 0;
                ans1 = calc(ans1, ans2, op);
                ans2 = 0;
                i++;
            }
        }
        else
        {
            if (isOperator(str[i]) && str[i + 1] != '(')
            {
                char op = str[i];
                i++;
                double token = tokenization(str);
                ans1 = calc(ans1, token, op);
            }
            if (str[i] == '\0')
                break;
            if (str[i] == '(')
            {
                flag = 1;
                op = str[i - 1];
                i++;
                ans2 = tokenization(str);
            }
            if (isOperator(str[i]) && str[i + 1] == '(')
                i++;
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
    char buf[100]; /* We will use this buffer for communication */

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
    printf("****** Socket created *****\n\n Waiting for client .....\n");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(20000);

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
    while (1)
    {

        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,
                           &clilen);

        if (newsockfd < 0)
        {
            perror("Accept error\n");
            exit(0);
        }
        printf("Client joined successfully \n");
        while (recv(newsockfd, buf, 21 , 0) >= 0)
        {
            double res = 0, curr_val = 0, res1 = 0 , dec = 0.1;
            int str_flag = 0, brckt_flag = 0, dec_flag = 0 ;
            char op1 = '!', op2 = '!';
            while (str_flag == 0)
            {
                for (int i = 0; i < 20; i++)
                {
                    if (buf[i] == ' ')
                        continue;
                    if (buf[i] == '\0')
                    {
                        str_flag = 1;
                        break;
                    }
                    if (brckt_flag == 0)
                    {
                        char ch = buf[i];
                        if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
                        {
                            if (op1 == '!')
                            {
                                op1 = ch;
                                res = curr_val;
                            }
                            else
                            {
                                res = calc(res, curr_val, op1);
                                op1 = ch;
                            }
                            curr_val = 0;
                            dec_flag = 0;
                            dec = 0.1;
                        }
                        else if (ch == '(')
                        {
                            op2 = op1;
                            op1 = '!' ;
                            brckt_flag = 1;
                        }
                        else 
                        {
                            if( dec_flag )
                            {
                                curr_val += dec*(ch - '0') ;
                                dec = dec/10 ;
                            }
                            else 
                            {
                                if( ch == '.')
                                {
                                    dec_flag = 1 ;
                                }
                                else curr_val = curr_val * 10 + ( ch - '0') ;
                            }
                        }
                    }
                    else 
                    {
                        char ch = buf[i] ;
                        if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
                        {
                            if (op1 == '!')
                            {
                                op1 = ch;
                                res1 = curr_val;
                            }
                            else
                            {
                                res1 = calc(res1, curr_val, op1);
                                op1 = ch;
                            }
                            curr_val = 0;
                            dec_flag = 0;
                            dec = 0.1;
                        }
                        else if (ch == ')')
                        {
                            if(op1 != '!')res1 = calc(res1 , curr_val , op1 ) ;
                            else res1 = curr_val ;
                            if(op2 != '!')res  = calc( res , res1 , op2 ) ;
                            else res = res1  ;
                            op2 = '!' ;
                            op1 = '!' ;
                            brckt_flag = 0;
                            curr_val = 0;
                            dec_flag = 0;
                            dec = 0.1;
                        }
                        else 
                        {
                            if( dec_flag )
                            {
                                curr_val += dec*(ch - '0') ;
                                dec = dec/10 ;
                            }
                            else 
                            {
                                if( ch == '.')
                                {
                                    dec_flag = 1 ;
                                }
                                else curr_val = curr_val * 10 + ( ch - '0') ;
                            }
                        }
                    }
                }
                for( int i = 0 ; i < 20 ; i++)buf[i] = '\0' ;
                if(str_flag == 0 )recv(newsockfd, buf, 21 , 0)  ;
            }
             if(op1 != '!')res = calc(res , curr_val , op1 ) ;
             else if( res == 0 )res = curr_val ;
            //  printf("result : %f\n",res) ;

             char result[100];
             sprintf(result, "%f", res);
             send(newsockfd, result, 100 , 0);
             printf("Result sent to the client : %f\n",res) ;
        }

        close(newsockfd);
    }
    return 0;
}
