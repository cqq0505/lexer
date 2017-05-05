#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE  *in;
FILE  *out;
FILE  *error;

char prog[500],token[50];
char ch;
int type;

int  p,m=0,n;
int row=1;
char *reserveTable[9]={"begin","end","integer","if","then","else","function","read","write"};



void input(char *prog)
{
    in =fopen("input.pas","r");
    if(in ==NULL)
    {
        printf("source file can not be found");
        exit(0);
    }

    int i=0;
    while(!feof(in))
    {
 	   ch = fgetc(in);
	   putchar(ch);
	   prog[i]=ch;
	   i++;
    }
    prog[--i]='#';

    fclose(in);
}

void output()
{
    if(out == NULL)
    {
        printf("output file can not be found");
        exit(0);

    }
    fprintf(out,"%16s %02d\n",token,type);
}


int letter(char c)//判断是否为字母
{
	if ((c >= 'a' && c <= 'z')||(c >= 'A' && c <= 'Z'))
	{
		return 1;

	}
	else
		return 0;
}

int digit(char c) //判断是否为数字
{
	if (c >= '0' && c <= '9')
		return 1;
	else
		return 0;
}

int reserve(char *p,char *table[])
{
    for(n=0;n<9;n++)
    {
        if(strcmp(p,table[n])==0)
        {
            type =n+1;
            return type;
        }

    }
    return 0;

}


void lexAnalyze()
{
     for(n=0;n<8;n++) token[n]=NULL;
	//过滤空格
    ch=prog[p++];
    while(ch==' ')
    {
        ch=prog[p];
        p++;
    }

    if(letter(ch))  //可能是标示符或者变量名
    {
        m=0;             //整形变量int
        while(letter(ch)||digit(ch))
        {
            token[m++]=ch;
            ch=prog[p++];
        }
		//识别出一个标示符
        token[m++]='\0';
		//回退一位
        p--;
        /*type=10;//symbol

        for(n=0;n<9;n++)
        {
            if(strcmp(token,reserveTable[n])==0)
            {	//字符串类别
                type=n+1;
                break;
            }
        }*/
        int val = reserve(token,reserveTable);
        if(val!=0)
        {
            type=val;//保留字

        }
        else type=10;//symbol


    }
    else if(digit(ch))  //数字
    {
             m=0;
            while(digit(ch))
            {
                token[m++]=ch;
                ch=prog[p++];
            }
		//回退一下
        p--;
        type=11;//constant



    }
    else switch(ch)   //其他字符
    {
        case'<':
			m=0;
			token[m++]=ch;
			//读取下一个字符
            ch=prog[p++];
            if(ch=='>')
            {
                type=13;//<>
                token[m++]=ch;
            }
            else if(ch=='=')
            {
                type=14;//<=
                token[m++]=ch;
            }
            else
            {
                type=15;//<
                p--;
            }
            break;
        case'>':
			m=0;
			token[m++]=ch;
			//读取下一个字符
            ch=prog[p++];
            if(ch=='=')
            {
                type=16;
                token[m++]=ch;
            }
            else
            {
                type=17;//>
                p--;
            }
            break;
        case':':
             m=0;
            token[m++]=ch;
            ch=prog[p++];
            if(ch=='=')
            {
                type=20;//:=
                token[m++]=ch;
            }
            else
            {
                //token[m++]=ch;
                type=-1;
                printf("error about assign\n");
                //fprintf(error,"%s错误",token);
                p--;

            }
            break;

		case'=':type=12;token[0]=ch;break;
        case'-':type=18;token[0]=ch;break;
        case'*':type=19;token[0]=ch;break;
        case'(':type=21;token[0]=ch;break;
        case')':type=22;token[0]=ch;break;
        case';':type=23;token[0]=ch;break;

        case'#':
            type=0;
            break;
        case'\n':
            row++;
            type=-2;
            break;
        default:
            token[0]=ch;
            type=-1;
            break;
    }

}
int main()
{
    p=0;
    row=1;
    input(prog);

    out = fopen("output.dyd","w");
    error= fopen("error.err","w");

    do
    {
        lexAnalyze();
        switch(type)
        {
            case 0:
                fprintf(out,"             EOF 25\n");
                break;

            case -1:
                fprintf(error,"LINE%d  %s符号不正确\n",row,token);
                break;

            case -2:
                //row =row++;
                fprintf(out,"            EOLN 24\n");
                break;
            default:
            //fprintf(out,"%16s %02d\n",token,type);
            output();
            printf("%16s %02d\n",token,type);
            break;

        }

    }
    while (type!=0);
    return 0;
}
