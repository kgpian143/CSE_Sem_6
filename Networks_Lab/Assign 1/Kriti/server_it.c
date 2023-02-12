// NETWORKS LAB (Assignent: 01, Qn: 02, SERVER PROCESS)
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

#define MAX_SIZE 100

struct valueStack
{
    double element[MAX_SIZE];
    int valueTop;
};

struct operatorStack
{
    char operator[MAX_SIZE];
    int operatorTop;
};

void pushValue(struct valueStack *s, double val)
{
    if (s->valueTop == MAX_SIZE - 1)
    {
        printf("\nStack Overflow\n");
        exit(0);
    }
    else
    {
        s->valueTop++;
        s->element[s->valueTop] = val;
    }
}

double popValue(struct valueStack *s)
{
    if (s->valueTop == -1)
    {
        printf("\nStack Underflow\n");
        exit(0);
    }
    else
    {
        double val = s->element[s->valueTop];
        s->valueTop--;
        return val;
    }
}

void pushOperator(struct operatorStack *s, char ch)
{
    if (s->operatorTop == MAX_SIZE - 1)
    {
        printf("\nStack Overflow\n");
        exit(0);
    }
    else
    {
        s->operatorTop++;
        s->operator[s->operatorTop] = ch;
    }
}

char popOperator(struct operatorStack *s)
{
    if (s->operatorTop == -1)
    {
        printf("\nStack Underflow\n");
        exit(0);
    }
    else
    {
        char ch = s->operator[s->operatorTop];
        s->operatorTop--;
        return ch;
    }
}

double applyOp(double a, double b, char op)
{
    switch (op)
    {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        return a / b;
    }
}

int Isoperator(char ch)
{
    if (ch == '*' || ch == '-' || ch == '/' || ch == '+')
        return 1;
    return 0;
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
        perror("Cannot create socket\n");
        exit(0);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(20000);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Unable to bind the local address\n");
        exit(0);
    }

    listen(sockfd, 5);
    printf("Waiting for a client..\n");

    while (1)
    {
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

        if (newsockfd < 0)
        {
            perror("Accept error\n");
            exit(0);
        }

        printf("\nConnected to a client\n");

        double val = 0, dec = 0.1;
        int flag = 0;
        struct valueStack value;
        value.valueTop = -1;
        // pushValue(&value, 0);
        struct operatorStack op;
        op.operatorTop = -1;

        int num_flag = 0;

        while (1)
        {
            for (i = 0; i < 100; i++)
                buf[i] = '\0';
            int n = recv(newsockfd, buf, 10, 0);

            if (n < 0)
            {
                perror("Unable to receive\n");
                exit(0);
            }

            if (strcmp("\0", buf) == 0)
            {
                printf("Received nothing: Client disconnected\n");
                printf("Waiting for a client..\n");
                break;
            }

            int i = 0, brac_flag = 0;
            double value1, value2;
            char ch;
            while (buf[i] != '\0' && i < n)
            {
                if (buf[i] == ' ')
                {
                    if (num_flag)
                    {
                        pushValue(&value, val);
                        num_flag = 0;
                    }
                    val = 0, dec = 0.1;
                    flag = 0;
                    i++;
                    continue;
                }
                else if (buf[i] == '(')
                {
                    if (num_flag)
                    {
                        pushValue(&value, val);
                        num_flag = 0;
                    }
                    brac_flag = 1;
                    val = 0, dec = 0.1;
                    flag = 0;
                    pushOperator(&op, '(');
                }
                else if (((buf[i] >= '0' && buf[i] <= '9') || buf[i] == '.'))
                {
                    num_flag++;

                    while (((buf[i] >= '0' && buf[i] <= '9') || buf[i] == '.'))
                    {
                        if (buf[i] == '\0')
                            break;
                        if (buf[i] == '.')
                        {
                            flag++;
                            i++;
                        }
                        if (flag)
                        {
                            val += ((buf[i] - '0') * dec);
                            dec /= 10;
                            i++;
                        }
                        else
                        {
                            val = (val * 10) + (buf[i] - '0');
                            i++;
                        }
                    }

                    i--;
                }
                else if (Isoperator(buf[i]))
                {
                    if (num_flag)
                    {
                        pushValue(&value, val);
                        num_flag = 0;
                    }
                    val = 0, dec = 0.1;
                    flag = 0;
                    if (op.operatorTop == -1 || op.operator[op.operatorTop] == '(')
                    {
                        pushOperator(&op, buf[i]);
                    }
                    else
                    {
                        // value1 = value.element[value.valueTop];
                        // popValue(&value);

                        // value2 = value.element[value.valueTop];
                        // popValue(&value);

                        // ch = op.operator[op.operatorTop];
                        // popOperator(&op);

                        if ((brac_flag == 1))
                        {
                            value1 = value.element[value.valueTop];
                            popValue(&value);

                            value2 = value.element[value.valueTop];
                            popValue(&value);

                            ch = op.operator[op.operatorTop];
                            popOperator(&op);

                            if( value.valueTop == -1 )
                            {
                                pushValue(&value , value2 ) ;
                                pushValue( &value , -value1 ) ;
                            }
                            else 
                            {
                                pushValue(&value, applyOp(value2, value1, ch));
                            }
                        }
                        else 
                        {
                            value1 = value.element[value.valueTop];
                            popValue(&value);
                             
                            ch = op.operator[op.operatorTop];
                            popOperator(&op);

                            if( value.valueTop == -1 )
                            {
                                pushValue( &value , -value1 ) ;
                            }
                            else 
                            {
                                value2 = value.element[value.valueTop];
                                popValue(&value);
                                pushValue(&value, applyOp(value2, value1, ch));
                            }
                        }
                        
                        pushOperator(&op, buf[i]);
                    }
                }

                else if (buf[i] == ')')
                {
                    if (num_flag)
                    {
                        pushValue(&value, val);
                        num_flag = 0;
                    }
                    val = 0, dec = 0.1;
                    flag = 0;
                    brac_flag = 01 ;
                    if (op.operator[op.operatorTop] != '(')
                    {
                        value1 = value.element[value.valueTop];
                        popValue(&value);

                        value2 = value.element[value.valueTop];
                        popValue(&value);

                        ch = op.operator[op.operatorTop];
                        popOperator(&op);

                        pushValue(&value, applyOp(value2, value1, ch));
                    }

                    popOperator(&op);
                }

                if (buf[i] != '\0')
                    i++;
            }

            if (buf[i - 1] == '\n')
            {
                if (num_flag)
                {
                    pushValue(&value, val);
                    num_flag = 0;
                }
                if (op.operatorTop != -1)
                {
                    value1 = value.element[value.valueTop];
                    popValue(&value);

                    value2 = value.element[value.valueTop];
                    popValue(&value);

                    ch = op.operator[op.operatorTop];
                    popOperator(&op);

                    pushValue(&value, applyOp(value2, value1, ch));
                    num_flag = 0;
                }

                printf("Final ans: %lf\n", value.element[value.valueTop]);
                val = 0;
                flag = 0;
                dec = 0.1;

                char ans[100];
                sprintf(ans, "%f", value.element[value.valueTop]);

                popValue(&value);
                // pushValue(&value, 0);

                send(newsockfd, ans, strlen(ans) + 1, 0);
                printf("Answer sent to client\n\n");
            }
        }

        close(newsockfd);
        printf("\n");
    }
    return 0;
}