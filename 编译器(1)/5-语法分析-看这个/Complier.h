/*
* ��ٱ����� VC 6.0 (Virtual Compiler 6.0)
*
* @author Underwater
* @1/12/2020
*
* ������������������������������
* �������C++����
* �ο��������ܹ���ö�����ͣ��ο��˿μ��ı��ʽ�ݹ��������
* ʣ�๦������ʵ��
* ���л���ΪMicrosoft Visual Studio Community 2019 �汾 16.4.2
*
* ���������������¡�������������
* ����while������ж�
* ����ע�͹���
* �����������������ж�
* ����if-else�߼��жϺ�����ƥ��
* etc.
*
* ������������ȱ�ݡ�������������
* ��֧��struct/����/ָ�뼰ָ����ز���
* ��֧�ָ����ͺ��ַ�������
* ��֧��++/--/��/||/&&
* ��֧��break��continue��������ڲ����߼��ж�
* ��֧�ּ��δ����ı����뺯����
* �����ڶ���δ������bug
* ������ע�ͺͲ����ֲ�
*/

#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<csetjmp>
#include<string>
#include <functional>
#include<vector>
#include<iostream>
#include<Windows.h>
#include<stack>
using namespace std;

#define IS_LETTER_OR_NUM(c) ('a'<=c&&c<='z'||'A'<=c&&c<='Z'||'0'<=c&&c<='9')
#define IS_LETTER(c) ('a'<=c&&c<='z'||'A'<=c&&c<='Z')
#define IS_NUM(c) ('0'<=c&&c<='9')
#define IS_TYPE(sym) (KW_CHAR<=sym&&sym<=KW_VOID)
#define MAX 1000


typedef struct TK {
	int token;
	string s;		
	int row;
}TkWord, * pTkWord;

enum TokenCode
{
	/* ��������ָ��� */
	TK_PLUS,		// + �Ӻ�
	TK_MINUS,		// - ����
	TK_MUL,			// * �˺�
	TK_DIVIDE,		// / ����
	TK_MOD,			// % ���������
	TK_ASSIGN,		// = ��ֵ����� 
	TK_EQ,			// == ���ں�
	TK_NEQ,			// != �����ں�
	TK_LT,			// < С�ں�
	TK_LEQ,			// <= С�ڵ��ں�
	TK_GT,			// > ���ں�
	TK_GEQ,			// >= ���ڵ��ں�
	TK_POINTSTO,	// -> ָ��ṹ���Ա�����
	TK_DOT,			// . �ṹ���Ա�����
	TK_AND,         // & ��ַ�������
	TK_OPENPA,		// ( ��Բ����
	TK_CLOSEPA,		// ) ��Բ����
	TK_OPENBR,		// [ ��������
	TK_CLOSEBR,		// ] ��������
	TK_BEGIN,		// { �������
	TK_END,			// } �Ҵ�����
	TK_SEMICOLON,	// ; �ֺ�    
	TK_COMMA,		// , ����
	TK_REF,			// //ע��
	TK_ELLIPSIS,	// ... ʡ�Ժ�
	TK_EOF,			// �ļ�������

	/* ���� */
	TK_CINT,		// ���ͳ���
	TK_CCHAR,		// �ַ�����
	TK_CSTR,		// �ַ�������

	/* �ؼ��� */
	KW_CHAR,		// char�ؼ���
	KW_SHORT,		// short�ؼ���
	KW_INT,			// int�ؼ���
	KW_VOID,		// void�ؼ���  
	KW_STRUCT,		// struct�ؼ���   
	KW_IF,			// if�ؼ���
	KW_ELSE,		// else�ؼ���
	KW_FOR,			// for�ؼ���
	KW_WHILE,		// while�ؼ���
	KW_CONTINUE,	// continue�ؼ���
	KW_BREAK,		// break�ؼ���   
	KW_RETURN,		// return�ؼ���
	KW_SIZEOF,		// sizeof�ؼ���

	KW_ALIGN,		// __align�ؼ���	
	KW_CDECL,		// __cdecl�ؼ��� standard c call
	KW_STDCALL,     // __stdcall�ؼ��� pascal c call

	/* ��ʶ�� */
	TK_IDENTV,//������
	TK_IDENTF//������
};

enum Error_type {
	PROGRAM_BEGIN,		//ִ���﷨����
	SUCC,				//�ɹ�
	SOMETHINGWRONG,		//δ֪����
	LACK_SEMICOLON,		//ȱ�ٷֺ�
	RETURN_ERROR,		//return���ʹ���
	BREAK_LACK_SEMICOLON,		//breakȱ�ٷֺ�
	CONTINUE_LACK_SEMICOLON,	//continueȱ�ٷֺ�
	EXTER_DEC,			//�ⲿ���������ִ���
	LACK_TYPE,			//ȱ������
	VRA_WRONG,			//��������
	FUNC_WRONG,			//��������
	LACK_IDENT,			//ȱ�ٱ�ʶ��
	LACK_OPENPA,		//ȱ����Բ����
	LACK_CLOSEPA,		//ȱ����Բ����
	LACK_OPENBR,		//ȱ����������
	LACK_CLOSEBR,		//ȱ����������
	LACK_BEGIN,			//ȱ��������
	LACK_END,			//ȱ���һ�����
	NESTED_FUNC_DECLEAR,//����Ƕ�׶���
	IF_LACK_OPENPA,		//ifȱ����Բ����
	IF_LACK_CLOSEPA,	//ifȱ����Բ����
	FOR_LACK_OPENPA,	//forȱ����Բ����
	FOR_LACK_CLOSEPA,	//forȱ����Բ����
	LACK_PRIMARY,		//���ȱ��ʽ����
	FUNCVAR_WRONG,		//�������ñ�������
	LACK_ST,			//ȱ�����
	CONTINUE_ERROR,		//continueʹ�ô���
	ELSE_ERROR,			//elseʹ�ô���
	FOR_ERROR,			//forʹ�ô���
	WHILE_LACK_OPENPA,	//whileȱ����Բ����
	WHILE_LACK_CLOSEPA,	//whileȱ����Բ����
	WHILE_ERROR,		//whileʹ�ô���
};

/*---------------------------------�ʷ���������----------------------------------------*/
void Print(string ch, int token);
void PrintWhite();
void tmpClera(pTkWord tmp);

/*---------------------------------�﷨��������----------------------------------------*/
void Compile();
void NextNode();
void External_dec();
void type_Statement();
void Variate(int i);
void Func();
void Func_Virate();
void Func_Body();
void Func_char();
void Func_int();
void Func_func();
void VariInFR();
void For_Process();
void If_Process();
void Else_Process();
void While_Process();
void Return_Process();
void PrintBegin();
void Primary_Assign();
void Next();
void E();
void C();
void T();
void F();