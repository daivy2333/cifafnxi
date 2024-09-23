#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#define maxline 1024


//全局变量
char keyword[][10] = {"auto", "bool", "break", "case", "char", "const", "continue", "default", "do"
, "double", "else", "enum", "extern", "float", "for","goto","if","int","long","main","register","return"
,"short","signed","sizeof","static","struct","switch","typedef","union","unsigned","void","volatile"
,"while","scanf","printf","include","define" };

char delimiter[][10] = {"(",")", "[","]" , "{","}", ".", ",", ";", "'", "#", "?", '"', };
char calculation[][10] = {"[+]", "[-]", "[*]", "[/]", "[%]", "[++]", "[--]", "[==]","[!=]", "[>]" , "[<]", "[>=]"
, "[<=]", "[&&]", "[||]", "[!]", "[=]", "[+=]", "[-=]", "[*=]", "[/=]", "[%=]", "[&=]", "[^=]", "[|=]", "[&]",  "[|]","[^]", "[~]", "[<<]", "[>>]"};
int nk=38 , nd=13, nc = 31;//num of 关键字界符运算符
//下面为偏移量
int ck = 1,cd = 70, cc = 39, ci = 83, cf = 84 , cn = 85;

bool pd_integer(char ch)
{
    if(ch >='0'&& ch<='9')return true;
    return false;
}

bool pd_character(char ch)
{
    if((ch>='a' && ch<='z') ||(ch>='A' && ch<='Z'))return true;
    return false;
}
int pd_keyword(char s[])
{
    for(int i=0;i<nk;i++)
    {
        if(strcmp(s,keyword[i])==0)return i;

    }
    return -1;
}

int pd_delimiter(char ch)
{
    for (int i = 0; i < nd; i++)
    {
        if (ch == delimiter[i][0])return i;
    }
    return -1;
    
}
int pd_calculation(char s[])
{
    for(int i=0;i<nc;i++)
    {
        if(strlen(calculation[i])-2==strlen(s))
        {
            bool flag =true;
            for(int j = 1,k=0;j<strlen(calculation[i])-1 && k< strlen(s);j++,k++)
            {
                if (calculation[i][j]!= s[k])
                {
                    flag = false;
                    break;
                }
                
            }
            if(flag)return i;
        }
    }
    return -1;
}

char* pd_spcode(int n)
{
    static char name[20];
    if (n>=1 &&n<=nk)strcpy(name,"关键字");
    else if (n>=cc && n<cd) strcpy(name,"运算符");
    else if (n >= cd && n < ci) strcpy(name, "界符");
	else if (n == ci) strcpy(name, "整型");
	else if (n == cf)strcpy(name, "浮点型");
	else if (n == cn) strcpy(name, "标识符");
	else strcpy(name, "未识别");
	return name;

}
int search_spcode(char s[])
{
    if(pd_character(s[0]))
    {
        if(pd_keyword(s) != -1)
        return pd_keyword(s) + ck;
        else
        return cn;
    }
    if (s[0] == '_')return cn;

    if(pd_integer(s[0]))
    {
        if(strstr(s, ".") != NULL)return cf;
        return ci;
    }

    if(strlen(s) == 1)
    {
        char ch =s[0];
        if (pd_delimiter(ch)!= -1)
        return pd_delimiter(ch)+cd;
    }

    if(pd_calculation(s)!=-1)
    return pd_calculation(s)+cc;

    return -1;
}

int main()
{
    char test[1030] = {0};
    FILE* fp =fopen("text.txt","r");
    if(fp==NULL)
    {
        printf("wrong select");
        return 0;
    }
    while (fgets(test,maxline,fp) != NULL)
    {
        int i=0,j=0;
        while (i<strlen(test))
        {
            if(test[i] == ' ' || test[i] == '\n' || test[i] == '\t')
            {
                i++;
                continue;
            }
            bool flag =true;
            char str[100] ={0};
            j=0;
            if(pd_integer(test[i]))
            {
                while(pd_integer(test[i]) || (test[i] == '.' && flag))
                {
                    if(test[i] == '.')flag=false;
                    str[j++] = test[i++];
                }
                i--;
            }
            else if(pd_character(test[i]) || test[i] == '_'){
                while (pd_character(test[i]) || test[i] == '_' || pd_integer(test[i]))str[j++] = test[i++];
                i--;
            }
            else if(test[i] == '+' ||test[i] == '-'||test[i] == '*'||test[i] == '/'||test[i] == '%'||test[i] == '='||test[i] == '|'||test[i] == '>'||test[i] == '<'||test[i] == '&'||test[i] == '!'||test[i] == '^')
            {
                if(test[i+1] =='=' ||test[i]==test[i+1])
                {
                    str[0]= test[i], str[1] = test[i+1];
                    i++;
                }
                else
                {
                    str[0]= test[i];
                }
            }
            else{
                str[0] =test[i];
            }
            i++;
            printf("( %s , %d )------%s\n", str, search_spcode(str), pd_spcode(search_spcode(str)));
        }
        
    }
    
    return 0;
}