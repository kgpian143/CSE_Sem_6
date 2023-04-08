#include <string>
#include <fstream>
#include <iostream>
using namespace std;

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
bool isEmpty( struct operatorstack * stack)
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
void push( struct operatorstack *stack , char op )
{
      struct operatorNode* n = (struct operatorNode*) malloc(sizeof(struct operatorNode));
        n->data = op;
        n->next = stack->top;
        stack->top = n;
}
int precedence(char ch ,  char ch1 ){
    int a , a1 ; 
    if( ch == '%')
    a = 4 ;
    else if(ch == '*' || ch=='/')
        a = 3;
    else if(ch == '+' || ch=='-')
        a = 2; 
    else
        a = 0;
    if( ch1 == '%')
    a1  = 4 ; 
    else if(ch1 == '*' || ch1 =='/')
        a1  = 3;
    else if(ch1  == '+' || ch1 =='-')
        a1 = 2; 
    else
        a1 = 0;
     if( a > a1 )return 1;
     else return 0 ;
}

int isOperator(char ch){
    if(ch=='+' || ch=='-' ||ch=='*' || ch=='/'|| ch == '%' || ch == '(' || ch == ')') 
        return 1;
    else
        return 0;
}
void formatingstring(char *s)
{
    for( int i = 0 ; s[i] != 0 ; i++ )
    {
        if( s[i] == ' ')
        {
            int j ;
            for( j = 0 ; s[i+1+j] == ' ' && s[i+j+1] != '\0' ; j++ );
            for( int k = i+1 ; s[k] != '\0' ; k++)s[k] = s[k+j] ;
        }
    }
}
char* convert(const char *infix) {
	struct operatorstack  *sp = createstack() ;
    int a ;
    for( a = 0 ; infix[a] != '\0' ; a++) ;
    sp->size = a; 
    char * postfix = (char *) malloc((sp->size) * sizeof(char));
    int i=0; // Track infix traversal
    int j = 0; // Track postfix addition 
    while (infix[i]!='\0')
    {
        if(!isOperator(infix[i])){
            postfix[j] = infix[i];
            j++;
            i++;
        }
        else{
            if( infix[i] == '(')
               {
                   push(sp,infix[i]);
                   i++ ;
               }
            else if( infix[i] == ')')
            {
                char c1 ;
                while(1)
                {
                   c1 =  pop(sp) ;
                   if( c1 == '(') break  ;
                   else 
                   {
                       postfix[j] = c1 ;
                       j++;
                   }
                   i++ ;
                }
            }
            else if(precedence(infix[i] , peek(sp))){
                push(sp, infix[i]);
                i++;
            }
            else{
                postfix[j] = pop(sp);
                j++;
            }
        }
    }
    while (!isEmpty(sp))    
    {
        postfix[j] = pop(sp);
        j++;
    }
    postfix[j] = '\0';
    formatingstring(postfix) ;
    cout<<postfix<<endl;
    return postfix;
}
int main() {
	string line;
	ifstream input_file("input.txt");
	ofstream part_1_output_file("20CS10074_A1_Q1_output.txt");
	if (input_file.is_open()) {
		while (getline(input_file,line)) {
			// First part: Implement convert function and associated linked list impl. of stack 
			char* rp_exp = convert(line.c_str());
		    part_1_output_file << rp_exp << endl;
		}
		input_file.close();
	}
	part_1_output_file.close();
	return 0;
}