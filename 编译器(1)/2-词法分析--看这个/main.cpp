#include <Windows.h>
#include "complier.h"

//ָ������: MAXKEY�� ָ��TkWord��ָ��;
TkWord *tk_HashTable[MAXKEY] = { 0 };//���ʵĹ�ϣ��
//����ַ����Ķ�̬����(vector())
vector<string> word;  // ���ָ���ַ��浽 word ��
//���handle��Ϊ����ɫ��ӡ
HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
//��ÿһ�з��Ž���token���룬���ж�Ӧ������ֵ
static TkWord keywords[]={ // ����
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

	{TK_CINT,"���γ���",NULL}, // 25
	{TK_CFLOAT,"�����ͳ���",NULL}, // 26
	{TK_CCHAR,"�ַ�����",NULL}, // 27
	{TK_CSTR,"�ַ�������",NULL}, // 28

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

//���ƣ�����̣ܶ�����
//���ƣ���ʵ����������������д�﷨������
int main(){
	// ����: �������ı��ֳ�һ�������� , ��������ɫ; 
	int keywordsLen = sizeof(keywords) / sizeof(TkWord); 
	InitKeywords(keywords, keywordsLen, tk_HashTable);//��ʼ����ϣ��, �����ش����ϣ��
	ifstream file;
	file.open("hashmap.c",ios::in);//FILE *fp=fopen("test.txt","r+");
	if(!file.is_open()){//�ļ���ʧ�ܣ�����
		return 0;
	}
	string str;//char str[1000];//��һ���ַ���
	
	while(getline(file,str)){//һ�ζ�ȡһ�У�fgets(str,sizeof(str),fp)
		if(str.empty()){
			continue;
		}
		word.clear();
		//split�ǰ�ÿһ�а���Ϊһ��һ���ĵ���
		int err_num=split(word,str);
		for(int i=0;i<word.size();i++){
			string temp=word[i];
			//cout<<temp;
			//pTkWord cur=tk_HashTable[elf_hash(temp)]; // �õ���Ӧ�ַ�����token
			pTkWord cur = find_(temp, tk_HashTable);

			if (NULL != cur && cur->spelling == temp) {//����hash table�е�����
				tokencode token = (tokencode)cur->tkcode;
				printColor(temp, token);
			}
			else if(temp[0]==34||isdigit(temp[0])||temp[0]==39){//���ͳ����������ͳ������ַ����������ַ�����
				SetConsoleTextAttribute(h,FOREGROUND_RED|FOREGROUND_GREEN);
				cout<<temp;
			}else{//�ո� ������ ���ߺ�����
				SetConsoleTextAttribute(h,FOREGROUND_INTENSITY);
				cout<<temp;
			}
		}
		cout<<endl;
	}
	system("pause");
	return 0;
}