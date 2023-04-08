/*
*******Networks Lab*******
* Name : Vinod Meena 
* Roll Number : 20CS10074
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int k;
int isOperator(char ch)
{
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%')
        return 1;
    else
        return 0;
}

double calc(double a, double b, char c)
{
    if (c == '+')
        return (b + a);
    else if (c == '-')
        return (b - a);
    else if (c == '*')
        return (b * a);
    else if (c == '/')
        return (b / a);
    else
        return 0;
}


struct operatorNode
{
    char data;
    struct operatorNode *next;
};
struct operatorstack
{
    int size;
    struct operatorNode *top;
};
struct operatorstack *createstack()
{
    struct operatorstack *stack;
    stack = (struct operatorstack *)malloc(sizeof(struct operatorstack));
    stack->top = NULL;
    return stack;
}
int isEmpty(struct operatorstack *stack)
{
    if (stack->top == NULL)
        return 1;
    else
        return 0;
}

char pop(struct operatorstack *stack)
{
    if (isEmpty(stack))
    {
        printf("Stack Underflow\n");
        return '\0';
    }
    else
    {
        struct operatorNode *n = stack->top;
        stack->top = stack->top->next;
        char x = n->data;
        free(n);
        return x;
    }
}
void push(struct operatorstack *stack, char op)
{
    struct operatorNode *n = (struct operatorNode *)malloc(sizeof(struct operatorNode));
    n->data = op;
    n->next = stack->top;
    stack->top = n;
}
struct valNode
{
    double data;
    struct valNode *next;
};
struct valstack
{
    int size;
    struct valNode *top;
};
struct valstack *createstack1()
{
    struct valstack *stack;
    stack = (struct valstack *)malloc(sizeof(struct valstack));
    stack->top = NULL;
    return stack;
}
void push1(struct valstack *stack, double val1)
{
    struct valNode *n = (struct valNode *)malloc(sizeof(struct valNode));
    n->data = val1;
    n->next = stack->top;
    stack->top = n;
}
int isEmpty1(struct valstack *stack)
{
    if (stack->top == NULL)
        return 1;
    else
        return 0;
}

double pop1(struct valstack *stack)
{
    if (isEmpty1(stack))
    {
        printf("Stack Underflow\n");
        return 0;
    }
    else
    {
        struct valNode *n = stack->top;
        stack->top = stack->top->next;
        double x = n->data;
        free(n);
        return x;
    }
}

int main()
{
    int sockfd, newsockfd; 
    int clilen;
    struct sockaddr_in cli_addr, serv_addr;

    int i;
    char buf[10]; 

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Cannot create socket\n");
        exit(0);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(20000);

    if (bind(sockfd, (struct sockaddr *)&serv_addr,
             sizeof(serv_addr)) < 0)
    {
        perror("Unable to bind local address\n");
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
            perror("Accept error\n");
            exit(0);
        }

        char *str;
        while (recv(newsockfd, buf, 11, 0) >= 0)
        {
            struct operatorstack *st_op = createstack();
            struct valstack *st_val = createstack1();
            int flag = 0, flag2 = 0;
            double ans = 0, val = 0, dec = 0.1;
            while ((flag == 0 ))
            {
                for (int i = 0; i < 10; i++)
                {
                    if (buf[i] == '\0')
                    {
                        flag = 1;
                        break;
                    }
                    if( buf[i] == ' ')continue ;
                    if (isOperator(buf[i]))
                    {
                        if (isEmpty(st_op) || st_op->top->data == '(')
                        {
                            push(st_op, buf[i]);
                            push1(st_val, val);
                        }
                        else
                        {
                            char op1 = pop(st_op);
                            if (isEmpty1(st_val))
                            {
                                push1(st_val , -val ) ;
                            }
                            else
                            {
                                double val2 = pop1(st_val);
                                push1(st_val, calc(val, val2, op1));
                            }
                            push(st_op , buf[i] );
                        }

                        
                        val = 0;
                        flag2 = 0;
                        dec = 0.1;
                    }
                    else if (buf[i] == '(')
                    {
                        push(st_op, buf[i]);
                        // printf("Operator %c pushed\n", buf[i]);
                    }
                    else if (buf[i] == ')')
                    {
                        char op1 = pop(st_op);
                        if (isEmpty1(st_val))
                        {
                            if(isEmpty(st_op))push1(st_val , val ) ;
                            else push1(st_val , -val ) ;
                        }
                        else if( op1 != '(')
                        {
                            // printf("pop value\n");
                            double val2 = pop1(st_val);
                            pop(st_op);
                            if( isEmpty(st_op) )push1(st_val, calc(val, val2, op1));
                            else 
                            {
                                val = calc(val, val2, op1 ) ;
                                op1 = pop(st_op) ;
                                val2 = pop1(st_val) ; 
                                push1(st_val, calc(val, val2, op1)); 
                            }
                        }
                        else if( op1 == '(')
                        {
                            op1 = pop(st_op) ;
                            double val2 = pop1(st_val) ; 
                            push1(st_val, calc(val, val2, op1)); 
                        }
                        val = 0;
                        flag2 = 0;
                        dec = 0.1;
                    }
                    else
                    {
                        if (buf[i] == '.')
                        {
                            flag2 = 1;
                        }
                        else if (flag2 == 0)
                        {
                            val = val * 10 + (buf[i] - '0');
                        }
                        else
                        {
                            val = val + dec * (buf[i] - '0');
                            dec /= 10;
                        }
                    }
                }
                for( int i = 0 ; i < 10 ; i++ )buf[i] = '\0' ;
                if(flag == 0) recv(newsockfd, buf, 11, 0) ;
            }
            if( isEmpty1(st_val))
            {
                if(isEmpty(st_op) != 1 )
                {
                    ans = -val ;
                }
                else 
                {
                    ans = val ;
                }
            }
            else if(isEmpty(st_op) != 1)
            {
                // printf("pop value\n");
                double val1 = pop1(st_val) ;
                // printf("pop operator\n");
                char op1 = pop(st_op) ;
                // printf("poped op %c\n", op1);
                ans = calc(val , val1 , op1 ) ;
            }
            else if(isEmpty(st_op) == 1)
            {
                // printf("pop value\n");
                double val1 = pop1(st_val) ;
                // printf("pop operator\n");
                // char op1 = pop(st_op) ;
                // printf("poped op %c\n", op1);
                ans = val1 ;
            }

            // printf("Expression from server : %s \n", str);
            char str_ans[50];
            // double ans = evaluate(str);
            // printf("%lf \n",ans) ;
            sprintf(str_ans, "%f", ans);
            char buff_send[50];
            strcpy(buff_send, str_ans);
            send(newsockfd, buff_send, strlen(buff_send) + 1, 0);
            printf("\nExpression evalutes : %s\n",buff_send) ;
            for (int i = 0; i < 50; i++)
                buff_send[i] = '\0';
        }
        close(newsockfd);
    }
    return 0;
}
