#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int i;
int isOperator(char ch)
{
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%')
        return 1;
    else
        return 0;
}
double tokenization(char *str)
{
    while( str[i] == ' ')i++ ;
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
    char str[100];
    printf("Enter the expression string : ");
    gets(str);
    // printf("%s", str);
    double ans = evaluate(str);
    printf("%f", ans);
}