#define _CRT_SECURE_NO_WARNINGS
#include "Complier.h"

extern jmp_buf jmpbuf;
extern int JmpJud;
extern int Row;
extern pTkWord pcur;
extern TkWord word[MAX];
extern int wordlen;
//ʹ����C++�������̣�����ʹ����ջ���ݽṹ�����﷨����
//���ļ�ʹ���˾���·��������������ֲ�Բ���
int main() {
	static TkWord keywords[] = {
	{TK_PLUS, "+"},
	{TK_MINUS, "-"},
	{TK_MUL, "*"},
	{TK_DIVIDE, "/"},
	{TK_MOD, "%"},
	{TK_EQ, "=="},
	{TK_NEQ, "!="},
	{TK_LT, "<"},
	{TK_LEQ, "<="},
	{TK_GT, ">"},
	{TK_GEQ, ">="},
	{TK_ASSIGN, "="},
	{TK_POINTSTO, "->"},
	{TK_DOT, "."},
	{TK_AND, "&"},
	{TK_OPENPA, "("},
	{TK_CLOSEPA, ")"},
	{TK_OPENBR, "["},
	{TK_CLOSEBR, "]"},
	{TK_BEGIN, "{"},
	{TK_END, "}"},
	{TK_SEMICOLON, ";"},
	{TK_COMMA, ","},
	{TK_ELLIPSIS, "..."},
	{TK_REF, "//"},
	{TK_EOF, "End_Of_File"},

	{TK_CINT, "���ͳ���"},
	{TK_CCHAR, "�ַ�����"},
	{TK_CSTR, "�ַ�������"},

	{KW_CHAR, "char"},
	{KW_SHORT, "short"},
	{KW_INT, "int"},
	{KW_VOID, "void"},
	{KW_STRUCT, "struct"},

	{KW_IF, "if"},
	{KW_ELSE, "else"},
	{KW_FOR, "for"},
	{KW_WHILE, "while"},
	{KW_CONTINUE, "continue"},
	{KW_BREAK, "break"},
	{KW_RETURN, "return"},
	{KW_SIZEOF, "sizeof"},
	{KW_ALIGN, "__align"},
	{KW_CDECL, "__cdecl"},
	{KW_STDCALL, "__stdcall"},
	{0}
	};
	vector <TkWord> hashtable[MAX];
	int keywordsLen = sizeof(keywords) / sizeof(TkWord);
	std::hash<std::string> h; // ʹ��std��hashtable
	int value = 0;

	for (int i = 0; i < keywordsLen - 1; ++i) {
		int value = h(keywords[i].s) % MAX;// ȡ�෨��ȡhash����;
		hashtable[value].push_back(keywords[i]);
	}

	int iRow = 1;
	char c[MAX] = { 0 };
	TkWord tmp;
	string s;
	FILE* fp = fopen("code.txt", "r");
	PrintBegin();
	/*------------------------------- �ʷ�����------------------------------------*/
	while (fgets(c, 1000, fp) != NULL) {
		printf("%-4d", iRow);
		int i = 0;
		while(i < strlen(c)){
			if (c[i] == ' ') {
				printf("%c", c[i++]);
			}
			else if (c[i] == '\n') {
				printf("%c", c[i++]);
			}
			else if (c[i] == '\t') {
				printf("%c", c[i++]);
			}
			else if (c[i] == '/' && c[i + 1] == '/') {
				s += c[i++];
				s += c[i++];
				tmp.token = TK_REF;
				tmp.s = s;
				tmp.row = iRow;
				word[wordlen++] = tmp;
				Print(s, 29);
				while (i < strlen(c)) { // ���ؼ��ֵ���ɫ��ӡ ע��
					string t;
					t += c[i];
					Print(t, 29);
					i++;
				}
				tmpClera(&tmp);  
				s.clear();
			}
			else if (IS_LETTER(c[i])) {//�����Сд��ĸ
				while (IS_LETTER_OR_NUM(c[i])) {//һֱ��
					s += c[i++];
				}
				tmp.s = s;
				tmp.row = iRow;
				value = h(s) % MAX;
				int j = 0;
				for(j=0;j<hashtable[value].size();j++)
					if (tmp.s == hashtable[value][j].s) {// ��hash��Ͱ���ҵ����ֵ
						tmp.token = hashtable[value][j].token;
						break;
					}
				if(tmp.token==0){
					tmp.token = TK_IDENTV;
				}
				//hashtable[value].push_back(tmp);
				word[wordlen++] = tmp;
				Print(s, tmp.token);
				tmpClera(&tmp);
				s.clear();
			}
			else if (IS_NUM(c[i])) {//���������
				while (IS_NUM(c[i])) {
					s += c[i++];
				}
				tmp.s = s;
				tmp.row = iRow;
				value = h(s) % MAX;
				int j = 0;
				for (j; j < hashtable[value].size(); j++)
					if (s == hashtable[value][j].s) {
						tmp.token = hashtable[value][j].token;
						break;
					}
				// û�ҵ�
				if (j == hashtable[value].size()) {
					tmp.token = TK_CINT;
					hashtable[value].push_back(tmp);
				}
				word[wordlen++] = tmp;
				Print(s, tmp.token);
				tmpClera(&tmp);
				s.clear();
			}
			else if (c[i] == 39) {//����дΪ�����űȽϺ�
				for (int k = 0; k < 3; k++)
					s += c[i++];
				tmp.token = TK_CCHAR;
				tmp.s = s;
				tmp.row = iRow;
				word[wordlen++] = tmp;
				Print(s, tmp.token);
				s.clear();
				tmpClera(&tmp);
			}
			else if (c[i] == 34) {//����дΪ˫���űȽϺ�
				s += c[i++];
				while (c[i] != 34) {
					s += c[i++];
				}
				s += c[i++];
				tmp.token = TK_CSTR;
				tmp.s = s;
				tmp.row = iRow;
				word[wordlen++] = tmp;
				Print(s, tmp.token);
				s.clear();
				tmpClera(&tmp);
			}
			else {
				if (c[i] == '=' && c[i+1] == '=') {
					s += c[i++];	s += c[i++];	tmp.token = TK_EQ;
				}else if (c[i] == '!' && c[i + 1] == '=') {
					s += c[i++];	s += c[i++];	tmp.token = TK_NEQ;
				}else if (c[i] == '<' && c[i + 1] == '=') {
					s += c[i++];	s += c[i++];	tmp.token = TK_LEQ;
				}else if (c[i] == '>' && c[i + 1] == '=') {
					s += c[i++];	s += c[i++];	tmp.token = TK_GEQ;
				}else if (c[i] == '-' && c[i + 1] == '>') {
					s += c[i++];	s += c[i++];	tmp.token = TK_POINTSTO;
				}else { s += c[i++]; }
				tmp.row = iRow;
				tmp.s = s; 
				value = h(s) % MAX;
				int j = 0;
				for (j; j < hashtable[value].size(); j++)
					if (s == hashtable[value][j].s) {
						tmp.token = hashtable[value][j].token;
						break;
					}
				Print(s, tmp.token);
				word[wordlen++] = tmp;
				s.clear();
				tmpClera(&tmp);
			}
		}
		iRow++;
		PrintWhite();
	}
	printf("\n");

	for (int i = 0; i < wordlen; i++) {
		if (word[i].token == TK_IDENTV && word[i + 1].token == TK_OPENPA)
			word[i].token = TK_IDENTF;
	}
	/*------------------------------- �﷨����------------------------------------*/
	string res;
	JmpJud = setjmp(jmpbuf);
	switch (JmpJud)
	{
	case SOMETHINGWRONG:res="����ʧ�ܣ�δ֪����";break;
	case LACK_SEMICOLON:res="����ʧ�ܣ�ȱ��';'��" ; break;
	case EXTER_DEC:res="����ʧ�ܣ��ⲿ������������" ; break;
	case LACK_TYPE:res="����ʧ�ܣ�ȱ�����ͷ���" ; break;
	case VRA_WRONG:res="����ʧ�ܣ���������" ; break;
	case FUNC_WRONG:res = "����ʧ�ܣ���������"; break;
	case LACK_IDENT:res="����ʧ�ܣ�ȱ�ٱ�ʶ����" ; break;
	case LACK_OPENPA:res="����ʧ�ܣ�ȱ��'('��" ; break;
	case LACK_CLOSEPA:res="����ʧ�ܣ�ȱ��' '��" ; break;
	case LACK_OPENBR:res="����ʧ�ܣ�ȱ��'['��" ; break;
	case LACK_CLOSEBR:res="����ʧ�ܣ�ȱ��']'��" ; break;
	case LACK_BEGIN:res="����ʧ�ܣ�ȱ��'{'��" ; break;
	case LACK_END:res="����ʧ�ܣ�ȱ��'}'��" ; break;
	case NESTED_FUNC_DECLEAR:res="����ʧ�ܣ�����Ƕ�׶��壡" ; break;
	case LACK_PRIMARY:res="����ʧ�ܣ����ȱ��ʽ����" ; break;
	case RETURN_ERROR:res="����ʧ�ܣ�return���ʹ���" ; break;
	case BREAK_LACK_SEMICOLON:res="����ʧ�ܣ�breakȱ��';'��" ; break;
	case CONTINUE_LACK_SEMICOLON:res="����ʧ�ܣ�continueȱ��';'��" ; break;
	case IF_LACK_OPENPA:res="����ʧ�ܣ�if��ȱ��'('��" ; break;
	case IF_LACK_CLOSEPA:res="����ʧ�ܣ�if��ȱ��')'!"; break;
	case FOR_LACK_OPENPA:res="����ʧ�ܣ�forȱ��'('��"; break;
	case FOR_LACK_CLOSEPA:res="����ʧ�ܣ�forȱ��')'��"; break;
	case FUNCVAR_WRONG:res = "����ʧ�ܣ��������ñ�������"; break;
	case LACK_ST:res = "����ʧ�ܣ�ȱ����䣡"; break;
	case CONTINUE_ERROR:res = "����ʧ�ܣ�continueʹ�ô���"; break;
	case ELSE_ERROR:res = "����ʧ�ܣ�elseʹ�ô���"; break;
	case FOR_ERROR:res = "����ʧ�ܣ�forʹ�ô���"; break;
	case WHILE_LACK_OPENPA:res = "����ʧ�ܣ�whileȱ����Բ���ţ�"; break;
	case WHILE_LACK_CLOSEPA:res = "����ʧ�ܣ�whileȱ����Բ���ţ�"; break;
	case WHILE_ERROR:res = "����ʧ�ܣ�whileʹ�ô���"; break;
	case PROGRAM_BEGIN:Compile();  // �﷨�����Ŀ�ʼ
	default:
		res="\n==========�ɹ� 1 ����ʧ�� 0 �������� 0 ��==========\n";
		break;
	}
	if (SUCC == JmpJud) {
		printf("============�������Ŀcode.c�ı��빤��=============");
		printf("\n%s", res.c_str());
		return 0;
	}
	else if (SOMETHINGWRONG == JmpJud) {
		printf("\n%s", res.c_str());
	}
	else {
		printf("\n==========%s==========\n", res.c_str());
		printf("\n=========== ����������%d ============\n", Row);
	}
}