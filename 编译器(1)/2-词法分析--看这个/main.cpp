#include <Windows.h>
#include "complier.h"

//指针数组: MAXKEY个 指向TkWord的指针;
TkWord *tk_HashTable[MAXKEY] = { 0 };//单词的哈希表
//存放字符串的动态数组(vector())
vector<string> word;  // 将分割的字符存到 word 中
//这个handle是为了颜色打印
HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
//把每一中符号进行token编码，都有对应的数字值
static TkWord keywords[]={ // 词素
	{TK_PLUS,"+",NULL}, // 0
	{TK_MINUS,"-",NULL}, // 1
	{TK_STAR,"*",NULL}, // 2
	{TK_DIVIDE,"/",NULL}, // 3
	{TK_MOD,"%",NULL}, // 4
	{TK_EQ,"==",NULL},//  5
	{TK_NEQ,"!=",NULL}, // 6
	{TK_LT,"<",NULL}, // 7
	{TK_LEQ,"<=",NULL}, // 8
	{TK_GT,">",NULL}, // 9
	{TK_GEQ,">=",NULL}, // 10
	{TK_ASSIGN,"=",NULL}, // 11
	{TK_POINTSTO,"->",NULL},// 12
	{TK_DOT,".",NULL}, // 13 
	{TK_AND,"&",NULL}, // 14
	{TK_OPENPA,"(",NULL}, // 15 
	{TK_CLOSEPA,")",NULL}, // 16
	{TK_OPENBR,"[",NULL}, // 17
	{TK_CLOSEBR,"]",NULL}, // 18
	{TK_BEGIN,"{",NULL}, // 19
	{TK_END,"}",NULL}, // 20
	{TK_SEMICOLON,";",NULL}, // 21
	{TK_COMMA,",",NULL}, // 22 
	{TK_ELLIPSIS,"...",NULL}, // 23	
	{TK_EOF,"End Of File",NULL}, // 24

	{TK_CINT,"整形常量",NULL}, // 25
	{TK_CFLOAT,"浮点型常量",NULL}, // 26
	{TK_CCHAR,"字符常量",NULL}, // 27
	{TK_CSTR,"字符串常量",NULL}, // 28

	{KW_CHAR,"char",NULL}, // 29
	{KW_SHORT,"short",NULL}, // 30
	{KW_INT,"int",NULL}, // 31
	{KW_VOID,"void",NULL}, // 32
	{KW_STRUCT,"struct",NULL}, // 33
	 
	{KW_IF,"if",NULL}, // 34
	{KW_ELSE,"else",NULL}, // 35
	{KW_FOR,"for",NULL}, // 36
	{KW_CONTINUE,"continue",NULL}, // 37
	{KW_BREAK,"break",NULL}, // 38
	{KW_RETURN,"return",NULL}, // 39
	{TK_IDENT}//40
};

//优势，代码很短，清晰
//劣势，其实他基础不错，还可以写语法分析的
int main(){
	// 主题: 将输入文本分成一个个词素 , 并标以颜色; 
	int keywordsLen = sizeof(keywords) / sizeof(TkWord); 
	InitKeywords(keywords, keywordsLen, tk_HashTable);//初始化哈希表, 将词素存入哈希表
	ifstream file;
	file.open("hashmap.c",ios::in);//FILE *fp=fopen("test.txt","r+");
	if(!file.is_open()){//文件打开失败，返回
		return 0;
	}
	string str;//char str[1000];//存一行字符串
	
	while(getline(file,str)){//一次读取一行，fgets(str,sizeof(str),fp)
		if(str.empty()){
			continue;
		}
		word.clear();
		//split是把每一行剥离为一个一个的单词
		int err_num=split(word,str);
		for(int i=0;i<word.size();i++){
			string temp=word[i];
			//cout<<temp;
			//pTkWord cur=tk_HashTable[elf_hash(temp)]; // 得到对应字符串的token
			pTkWord cur = find_(temp, tk_HashTable);

			if (NULL != cur && cur->spelling == temp) {//是在hash table中的内容
				tokencode token = (tokencode)cur->tkcode;
				printColor(temp, token);
			}
			else if(temp[0]==34||isdigit(temp[0])||temp[0]==39){//整型常量、浮点型常量、字符串常量、字符常量
				SetConsoleTextAttribute(h,FOREGROUND_RED|FOREGROUND_GREEN);
				cout<<temp;
			}else{//空格 变量名 或者函数名
				SetConsoleTextAttribute(h,FOREGROUND_INTENSITY);
				cout<<temp;
			}
		}
		cout<<endl;
	}
	system("pause");
	return 0;
}