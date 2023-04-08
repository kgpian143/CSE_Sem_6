#include<stdio.h>
#include<stdlib.h>
#include<string.h>
// #include<stack.h>
struct operatorNode
{
    char data ;
    struct operatorNode *next ;
};
struct operatorstack
{
    int size ;
    struct operatorNode* top ;
};
struct operatorstack *createstack()
{
    struct operatorstack *stack  ;
    stack = ( struct operatorstack *)malloc(sizeof( struct operatorstack ));
    stack->top = NULL ;
    return stack ;
}
int isEmpty( struct operatorstack * stack)
{
    if( stack->top == NULL )return 1 ;
    else return 0 ;
}
char peek(struct operatorstack *stack )
{
    if(isEmpty(stack)){
        return '\0' ;
    }
    else 
    {
        return (stack->top)->data ;
    }
}
char pop( struct operatorstack *stack )
{
    if(isEmpty(stack)){
        printf("Stack Underflow\n");
        return '\0' ;
    }
    else 
    {
        struct operatorNode* n = stack->top ;
        stack->top = stack->top->next;
        char x = n->data;
        free(n);
        return x;
    }
}

struct valNode
{
    double data ;
    struct valNode *next ;
};
struct valstack
{
    int size ;
    struct operatorNode* top ;
};
struct valstack *createstack1()
{
    struct valstack *stack  ;
    stack = ( struct valstack *)malloc(sizeof( struct valstack ));
    stack->top = NULL ;
    return stack ;
}
int isEmpty( struct valstack * stack)
{
    if( stack->top == NULL )return 1 ;
    else return 0 ;
}

double pop( struct valstack *stack )
{
    if(isEmpty(stack)){
        printf("Stack Underflow\n");
        return 0 ;
    }
    else 
    {
        struct operatorNode* n = stack->top ;
        stack->top = stack->top->next;
        double x = n->data;
        free(n);
        return x;
    }
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

int isOperator(char ch)
{
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%')
        return 1;
    else
        return 0;
}
int main()
{
    char buf[10] ;
    char ch ;
    ch = getchar() ;
    int flag = 0 , flag1 = 0 ;
    int k = 0 ;
    double ans = 0  , val = 0  ;
    struct valstack *st1 = createstack1() ; 
    struct operatorstack *st2 = createstack() ; 
    while( flag == 0 )
    {
        buf[k++] = ch ; 
        if( k == 10 && ch == '\n')
        {
            int f1 = 0 ;
            for( int i = 0 ; i < 10 ; i++ )
            {
                if( isOperator(buf[i]))
                {
                    
                }
            }
        }
    }
}