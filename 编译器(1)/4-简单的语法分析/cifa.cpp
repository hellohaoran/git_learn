#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct{
	char first[20];//用来存储符号串
	int second;//用来存储符号串对应的TOKEN值
}Grammar_t;
char expr[50] = "(1.5+5.789)*82-10/2+9";//需要进行语法分析的算术表达式
Grammar_t word[100]={0};//用于存储每个符号串极其TOKEN值
int wordLen=0;//用于存储最终word里有什么样的 符号串 
int idx = 0;//代表到达了第几个符号串，数组下标从零开始，
int sym;//存储当前符号串的TOKEN值
int err = 0; //错误标识

void T();
void F();
//词法分析
int word_analysis(Grammar_t* word, const char* expr)
{
	unsigned int i;
	for(i=0; i<strlen(expr); ++i)
	{
		// 如果是 + - * / ( )
		if(expr[i] == '(' || expr[i] == ')' || expr[i] == '+' 
			|| expr[i]  == '-' || expr[i] == '*' || expr[i] == '/')
		{
			char tmp;
			tmp=expr[i];
			switch (expr[i])
			{
				case '+'://对应1结束状态
					word[wordLen].first[0]=expr[i]; // 一个字符就是 直接赋值;
					word[wordLen].second=1;
					wordLen++;
					break;
				case '-'://对应2结束状态
					word[wordLen].first[0]=expr[i];
					word[wordLen].second=2;
					wordLen++;
					break;
				case '*'://对应3结束状态
					word[wordLen].first[0]=expr[i];
					word[wordLen].second=3;
					wordLen++;
					break;
				case '/'://对应4结束状态
					word[wordLen].first[0]=expr[i];
					word[wordLen].second=4;
					wordLen++;
					break;
				case '('://对应9结束状态
					word[wordLen].first[0]=expr[i];
					word[wordLen].second=6;
					wordLen++;
					break;
				case ')'://对应10结束状态
					word[wordLen].first[0]=expr[i];
					word[wordLen].second=7;
					wordLen++;
					break;
			}
		}
		// 如果是数字开头
		else if(expr[i]>='0' && expr[i]<='9')
		{
			char tmp[50]={0};
			int k=0;
			while(expr[i]>='0' && expr[i]<='9')//对应状态5
			{
				tmp[k++]=expr[i];
				++i;
			}
			if(expr[i] == '.')//对应状态7
			{
				++i; // 遇到. 跳过;
				if(expr[i]>='0' && expr[i]<='9')
				{
					tmp[k++]='.';
					while(expr[i]>='0' && expr[i]<='9')//对应状态状态8
					{
						tmp[k++]=expr[i];
						++i;
					}
				}
				else  
				{
					return -1;  // .后面不是数字，词法错误，对应状态6
				}
			}
			strcpy(word[wordLen].first,tmp);
			word[wordLen].second=5;
			wordLen++;
			--i;
		}
		// 如果以.开头
		else  
		{
			return -1;  // 以.开头，词法错误，对应状态6
		}
	}
	return 0;
}

void Next() // 推进对存储的表达式的读取;
{
	if(idx < wordLen)
		sym = word[idx++].second;
	else
		sym = 0;  // 如果读完毕了 token 设置为0;
}
 // 递归下降分析;   使用long jump 来 解决程序出现bug 
// E → T { +T | -T } 
void E() // 用递归文法分析;
{
	T();
	while(sym == 1 || sym == 2) // token 是加号或者 减号; 
	{
		Next(); // sym 进行变化
		T();
	}
}

// T → F { *F | /F } 
void T()
{
	F();
	while(sym == 3 || sym == 4) // token 为* 或者 /
	{
		Next();
		F();
	}
}

// F → (E) | d
void F()  // 在F结束递归; 因为 F是数字或者右括号结束; 一个表达式也是如此;
{
	if (sym == 5) // 如果是数字就往后走 ,d 代表数字
	{
		Next(); 
	}
	else if(sym == 6) //  如果是左括号; 
	{
		Next(); 
		E();  // 就吃括号里面东西;
		if (sym == 7) // 如果是右括号就停止
		{
			Next();
		}
		else
		{
			err = -1;
		}
	}
	else
	{
		err = -1;
		puts("Wrong Expression.");
		system("pause");
	}
}

int main()
{	
	int i;
	//对表达式进行词法分析
	int err_num = word_analysis(word, expr); ///用于存储每个符号串和TOKEN值
	if (-1 == err_num)
	{
		puts("Word Error!");
	}
	else
	{
		// 测试输出
		for(i=0;i<wordLen;i++)
		{
			printf("%s %d\n",word[i].first,word[i].second);
		}
		// 词法正确，进行语法分析
		Next();
		E();//开始语法分析
		if (sym == 0 && err == 0)  // 注意要判断两个条件
			printf("%s is Right Expression.\n",expr);
		else
			printf("%s is Wrong Expression.\n",expr);
	}
	system("pause");
	return 0;
}