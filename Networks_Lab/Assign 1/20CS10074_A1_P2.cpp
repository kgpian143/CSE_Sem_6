#include <string>
#include <fstream>
#include <iostream>
using namespace std;

struct operatorNode
{
    int data ;
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
bool isEmpty( struct operatorstack * stack)
{
    if( stack->top == NULL )return 1 ;
    else return 0 ;
}
int peek(struct operatorstack *stack )
{
    if(isEmpty(stack)){
        return 0 ;
    }
    else 
    {
        return (stack->top)->data ;
    }
}
int pop( struct operatorstack *stack )
{
    if(isEmpty(stack)){
        printf("Stack Underflow\n");
        return 0 ;
    }
    else 
    {
        struct operatorNode* n = stack->top ;
        stack->top = stack->top->next;
        int x = n->data;
        free(n);
        return x;
    }
}
void push( struct operatorstack *stack , int op )
{
      struct operatorNode* n = (struct operatorNode*) malloc(sizeof(struct operatorNode));
        n->data = op;
        n->next = stack->top;
        stack->top = n;
}
int isOperator(char ch){
    if(ch=='+' || ch=='-' ||ch=='*' || ch=='/'|| ch == '%') 
        return 1;
    else
        return 0;
}
int calc( int a , int b , const char c)
{
     if( c == '+')return (a+b) ;
     else if( c == '-')return (a-b) ;
     else if( c == '*')return (a*b) ;
     else if( c == '/')return (a/b) ;
     else if( c == '%')return (a%b) ;
     else return 0 ;
}
int evaluate(const char* rp_exp) {
    struct operatorstack * sp = (struct operatorstack *) malloc(sizeof(struct operatorstack));
    int cnt = 0 ;
	for( int i = 0 ; rp_exp[i] != '\0'  ;i++ )
    {
        if( rp_exp[i] == ' ')continue; 
        else if(isOperator(rp_exp[i]) == 0 )
        {
            int c1 = 0  ;
            for( ; rp_exp[i] != ' ' && rp_exp[i] !='\0' ; i++ )
            {
                c1 = 10*c1 + rp_exp[i] - '0' ;
            }
            push( sp , c1 );
        }  
        else
        {
            int a ,  b , c ;
            b = pop(sp) ;
            a = pop(sp) ;
            c = calc( a , b , rp_exp[i]);
            cnt =  c ;
            push(sp , c);
        }
    }
    cout <<cnt<<endl;
    free(sp) ;
	return cnt;
}
int main() {
	string line;
	ifstream input_file("20CS10074_A1_Q1_output.txt");
	ofstream part_2_output_file("20CS10074_A1_Q2_output.txt");
	if (input_file.is_open()) {
		while (getline(input_file,line)) {
			// Second part: Implement evaluate function and associated linked list impl. of stack
			part_2_output_file << evaluate(line.c_str()) << endl;
		}
		input_file.close();
	}
	part_2_output_file.close();
	return 0;
}
