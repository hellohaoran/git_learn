#include "Complier.h"

jmp_buf jmpbuf;
int JmpJud;//�Ƿ����ɹ��ı�־
int Row;
pTkWord pcur;//ָ��ʼ��Ҫ�����Ĵ�
TkWord word[MAX] = { 0 };//�Ѵ������Ϊһ��һ���ʣ��浽word�ṹ������
int wordlen;
int pend = 0, err = 0;
string FT;
stack <string> st;		//�洢{
stack <string> stelse;	//�洢 if �������else
stack <string> expre;		//���ʽ�ڲ�������ƥ��

/*---------------------------------�ʷ���������----------------------------------------*/
void Print(string ch, int token) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	if (token >= TK_IDENTV)
		SetConsoleTextAttribute(h, FOREGROUND_INTENSITY);//������ɫ
	else if (token >= KW_CHAR)
		SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);//�ؼ�����ɫ
	else if (token >= TK_CINT)
		SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN);//������ɫ
	else
		SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_INTENSITY);
	if (-1 == ch[0]) {
		printf("\n	End_Of_File!");
		SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	}
	cout << ch;
}

void PrintWhite() {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}

void tmpClera(pTkWord tmp) {
	tmp->token = 0;
	tmp->row = 0;
	tmp->s.clear();
}

/*---------------------------------�ʷ���������----------------------------------------*/
//���뺯��
void Compile() {
	JmpJud = SUCC;
	pcur = word;
	External_dec();
}

void NextNode() {
	pcur++;
}

//�ⲿ����
void External_dec() {   //�˳��ⲿ����ʱ pcurΪ��ǰ������:������} 
	if (pcur == word + wordlen) {//�ߵ����ͽ�����
		longjmp(jmpbuf, SUCC);
		return;
	}
	else if (pcur->token == TK_REF) {//�Ƿ���ע��
	}
	else if (pcur->token== TK_SEMICOLON) {//�Ƿ��Ƿֺ�
		Row = pcur->row;
		longjmp(jmpbuf, EXTER_DEC);
	}
	else if (pcur->token == TK_END) {//�Ƿ����Ҵ�����
		Row = pcur->row;
		longjmp(jmpbuf, LACK_BEGIN);
	}
	else if (!IS_TYPE(pcur->token)) {//�Ƿ�������
		Row = pcur->row;
		longjmp(jmpbuf, LACK_TYPE);
	}
	else {
		type_Statement();
	}
	NextNode();
	External_dec();
}

//�ⲿ�������� ����/����  ���� pcurΪ�ֺ��˳�   ���� pcurΪ�ֺ�or}�˳�
void type_Statement() {
	if ((pcur+1)->token == TK_IDENTV) {//�Ƿ��Ǳ�����
		int i = pcur->token;
		NextNode();
		NextNode();
		Variate(i);
	}
	else if ((pcur + 1)->token == TK_IDENTF) {//�Ƿ��Ǻ�����
		Func();
	}
	else {
		Row = pcur->row;
		longjmp(jmpbuf, EXTER_DEC);
	}
}

//�Ǳ�����  pcurΪ��������
void Variate(int i) {
	if (pcur->token == TK_SEMICOLON) {
		return;
	}
	else if (pcur->token == TK_ASSIGN) {
		NextNode();
		switch (i) {
		case KW_INT:
			if (pcur->token == TK_CINT) {//�����������ĳ���������һ�µ�
				NextNode();
				Variate(i);
			}
			else {
				Row = pcur->row;
				longjmp(jmpbuf, VRA_WRONG);
			}
			break;
		case KW_CHAR:
			if (pcur->token == TK_CCHAR|| pcur->token == TK_CINT) {
				NextNode();
				Variate(i);
			}
			else {
				Row = pcur->row;
				longjmp(jmpbuf, VRA_WRONG);
			}
			break;
		default:
			Row = pcur->row;
			longjmp(jmpbuf, VRA_WRONG);
			break;
		}
	}
	else if (pcur->token == TK_COMMA) {
		NextNode();
		if (pcur->token != TK_IDENTV) {
			Row = pcur->row;
			longjmp(jmpbuf, VRA_WRONG);
		}
		else {
			NextNode();
			Variate(i);
		}
	}
	else {
		Row = pcur->row;
		longjmp(jmpbuf, LACK_SEMICOLON);
	}
}

//�Ǻ�����  ��ʱpcurΪint/����
void Func() {
	FT = pcur->s;//FT�Ǻ�������ֵ����
	NextNode();
	NextNode();
	if (pcur->token != TK_OPENPA) {
		Row = pcur->row;
		longjmp(jmpbuf, LACK_OPENPA);
	}
	else {  //��������
		NextNode();
		if (pcur->token == TK_CLOSEPA) {  //��������
			NextNode();
		}
		else {  //����������  ���β� ����  
			Func_Virate();
			NextNode();
		}
		if (pcur->token == TK_SEMICOLON) { //�Ƿֺ� Ϊ�������� return
			return;
		}
		else if(pcur->token == TK_BEGIN){  //��{ �������� ֱ������pcurΪ}
			Func_Body();
			if ((pcur + 1)->token == TK_SEMICOLON) {
				NextNode();
			}
			return;
		}
		else {
			Row = pcur->row;
			longjmp(jmpbuf, FUNC_WRONG);
		}
	}
}

//�����βα������� pcurΪ��һ��  ����)����
void Func_Virate() {
	if (!IS_TYPE(pcur->token)) {
		Row = pcur->row;
		longjmp(jmpbuf, FUNC_WRONG);
	}
	else {
		NextNode();
		if (pcur->token < TK_IDENTV) {
			Row = pcur->row;
			longjmp(jmpbuf, FUNC_WRONG);
		}
		NextNode();
		if (pcur->token == TK_CLOSEPA)//�����žͽ���
			return;
		else if(pcur->token == TK_COMMA){//���Ŵ�������һ���β�
			NextNode();
			Func_Virate();
		}
		else {
			Row = pcur->row;
			longjmp(jmpbuf, FUNC_WRONG);
		}
	}
}

//�����崦�� pcurΪ{    ���� }�˳�
void Func_Body() {
	st.push(pcur->s);
	NextNode();
	if (pcur->token == TK_END) {//����Ϊ�� ֱ�ӷ���
		st.pop(); 
		return;
	}
	else {  
		while (!st.empty()) {
			switch (pcur->token)//pcurΪ��������(��������һ��{ )��ÿ�����ַ�
			{
			case TK_REF:
				NextNode();
				break;
			case KW_CHAR:
				NextNode();
				Func_char();
				break;
			case KW_INT:
				NextNode();
				Func_int();
				NextNode();
				break;
			case KW_IF:
				If_Process();
				break;
			case KW_ELSE:
				Else_Process();
				break;
			case KW_FOR:
				For_Process();
				break;
			case KW_WHILE:
				While_Process();
				break;
			case KW_RETURN:
				NextNode();
				Return_Process();
				break;
			case TK_BEGIN:
				st.push(pcur->s);
				NextNode();
				break;
			case TK_END:
				if (st.top()=="{") {
					st.pop();
					if (st.size() > 0)
						NextNode();
					break;
				}
				else {
					Row = pcur->row;
					longjmp(jmpbuf, LACK_BEGIN);
				}
			case TK_IDENTV:
				Primary_Assign();
				NextNode();
				break;
			case TK_IDENTF:
				Func_func();
				break;
			case KW_CONTINUE:
				NextNode();
				if (pcur->token != TK_SEMICOLON) {
					Row = pcur->row;
					longjmp(jmpbuf, CONTINUE_LACK_SEMICOLON);
				}
				NextNode();
				break;
			case KW_BREAK:
				NextNode();
				if (pcur->token != TK_SEMICOLON) {
					Row = pcur->row;
					longjmp(jmpbuf, BREAK_LACK_SEMICOLON);
				}
				NextNode();
				break;
			default:
				Row = pcur->row;
				longjmp(jmpbuf, SOMETHINGWRONG);
				break;
			}
		}
	}
	while (!st.empty())
		st.pop();
	while (!stelse.empty())
		stelse.pop();
}

void Func_char() { //���������ڲ���char ��ǰpcurΪchar����ַ�
	if (pcur->token == TK_IDENTV) {
		NextNode();
		if (pcur->token == TK_ASSIGN) {
			NextNode();
			if (pcur->token == TK_CCHAR|| pcur->token== TK_CINT) {
				NextNode();
				if (pcur->token == TK_COMMA) {
					NextNode();
					Func_char();
				}
				else if (pcur->token == TK_SEMICOLON) {
					NextNode();
					return;
				}
				else {
					Row = pcur->row;
					longjmp(jmpbuf, VRA_WRONG);
				}
			}
			else {
				Row = pcur->row;
				longjmp(jmpbuf, VRA_WRONG);
			}
		}
		else if (pcur->token == TK_COMMA) {
			NextNode();
			Func_char();
		}
		else if (pcur->token == TK_SEMICOLON) {
			NextNode();
			return;
		}
		else {
			Row = pcur->row;
			longjmp(jmpbuf, VRA_WRONG);
		}
	}
	else {
		Row = pcur->row;
		longjmp(jmpbuf, VRA_WRONG);
	}
}

void Func_int() { //���������ڲ���int ��ǰpcurΪint����ַ�
	if (pcur->token == TK_IDENTV) {
		Primary_Assign();
		while (pcur->s == ",") {
			NextNode();
			Primary_Assign();
		}
	}
	else {
		Row = pcur->row;
		longjmp(jmpbuf, VRA_WRONG);
	}
}

void Func_func() {  
	NextNode();
	if (pcur->token != TK_OPENPA) {
		Row = pcur->row;
		longjmp(jmpbuf, FUNC_WRONG);
	}
	NextNode();
	if (pcur->token == TK_CLOSEPA) {
		NextNode();
		if (pcur->token != TK_SEMICOLON) {
			if (pcur->token == TK_BEGIN) {
				Row = pcur->row;
				longjmp(jmpbuf, NESTED_FUNC_DECLEAR);
			}
			Row = pcur->row;
			longjmp(jmpbuf, LACK_SEMICOLON);
		}
		NextNode();
		return;
	}
	else VariInFR();
}

void VariInFR() { //    �������ò����ж� pcurΪ��ǰ��һ������
	if (pcur->token == TK_CINT || pcur->token == TK_IDENTV|| pcur->token == TK_CSTR) {
		NextNode();
		if (pcur->token == TK_COMMA) {
			NextNode();
			VariInFR();
		}
		else if (pcur->token == TK_CLOSEPA) {
			NextNode();
			if (pcur->token != TK_SEMICOLON) {
				Row = pcur->row;
				longjmp(jmpbuf, LACK_SEMICOLON);
			}
			NextNode();
			return;
		}
		else {
			Row = pcur->row;
			longjmp(jmpbuf, FUNCVAR_WRONG);
		}
	}
	else {
		Row = pcur->row;
		longjmp(jmpbuf, FUNCVAR_WRONG);
	}
}

void If_Process(){
	stelse.push("if");
	NextNode();
	if (pcur->token != TK_OPENPA) {
		Row = pcur->row;
		longjmp(jmpbuf, IF_LACK_OPENPA);
	}
	NextNode();
	Primary_Assign();
	if (pcur->token != TK_CLOSEPA) {
		Row = pcur->row;
		longjmp(jmpbuf, IF_LACK_CLOSEPA);
	}
	NextNode();
	if (pcur->token == KW_ELSE) {
		Row = pcur->row;
		longjmp(jmpbuf, LACK_ST);
	}
	else if (pcur->token == TK_END) {
		Row = pcur->row;
		longjmp(jmpbuf, LACK_ST);
	}
	else if (pcur->token == TK_SEMICOLON) {
		NextNode();
	}
	else if (pcur->token == TK_BEGIN) {
		st.push(pcur->s);
		NextNode();
	}
	else return;
}

void Else_Process(){
	if (stelse.empty()) {
		Row = pcur->row;
		longjmp(jmpbuf, ELSE_ERROR);
	}
	stelse.pop();
	NextNode();

	if (pcur->token == TK_END) {
		Row = pcur->row;
		longjmp(jmpbuf, LACK_ST);
	}
	else if (pcur->token == TK_SEMICOLON) {
		NextNode();
	}
	else if (pcur->token == TK_BEGIN) {
		st.push(pcur->s);
		NextNode();
	}
	else return;
}

void For_Process() {
	int count = 0;
	NextNode();
	if (pcur->token != TK_OPENPA) {
		Row = pcur->row;
		longjmp(jmpbuf, FOR_LACK_OPENPA);
	}
	NextNode();	//��һ�����ʽ
	if (pcur->token == TK_SEMICOLON) {
		count++;
		NextNode();
	}
	else {
		if (IS_TYPE(pcur->token)) {
			NextNode();
		}
		Primary_Assign();
		if (pcur->token != TK_SEMICOLON) {
			Row = pcur->row;
			longjmp(jmpbuf, FOR_ERROR);
		}
		count++;
		NextNode();
	}	//�ڶ������ʽ
	if (pcur->token == TK_SEMICOLON) {
		count++;
		NextNode();
	}
	else {
		Primary_Assign();
		if (pcur->token != TK_SEMICOLON) {
			Row = pcur->row;
			longjmp(jmpbuf, FOR_ERROR);
		}
		count++;
		NextNode();
	}//���������ʽ
	if (pcur->token == TK_CLOSEPA) {
		if (count != 2) {
			Row = pcur->row;
			longjmp(jmpbuf, FOR_ERROR);
		}
		NextNode();
	}
	else {
		Primary_Assign();
		if (pcur->token != TK_CLOSEPA) {
				Row = pcur->row;
				longjmp(jmpbuf, FOR_LACK_CLOSEPA);
		}
		if (count != 2) {
			Row = pcur->row;
			longjmp(jmpbuf, FOR_ERROR);
		}
		NextNode();
	}// for(;;)�Ѵ������ pcurΪ��һ���ַ�
	if (pcur->token == KW_CONTINUE) {
		NextNode();
		if (pcur->token != TK_SEMICOLON) {
			Row = pcur->row;
			longjmp(jmpbuf, CONTINUE_LACK_SEMICOLON);
		}
		NextNode();
		return;
	}
	else if (pcur->token == KW_BREAK) {
		NextNode();
		if (pcur->token != TK_SEMICOLON) {
			Row = pcur->row;
			longjmp(jmpbuf, BREAK_LACK_SEMICOLON);
		}
		NextNode();
		return;
	}
	else if (pcur->token == TK_END) {
		Row = pcur->row;
		longjmp(jmpbuf, LACK_ST);
	}
	else if (pcur->token == TK_SEMICOLON) {
		NextNode();
	}
	else if (pcur->token == TK_BEGIN) {
		st.push(pcur->s);
		NextNode();
	}
	else return;
}

void While_Process(){
	NextNode();
	if (pcur->token != TK_OPENPA) {
		Row = pcur->row;
		longjmp(jmpbuf, WHILE_LACK_OPENPA);
	}
	NextNode();
	if (pcur->token == TK_CLOSEPA) {
		Row = pcur->row;
		longjmp(jmpbuf, WHILE_ERROR);
	}
	Primary_Assign();
	if (pcur->token != TK_CLOSEPA) {
		Row = pcur->row;
		longjmp(jmpbuf, WHILE_ERROR);
	}
	NextNode();
	if (pcur->token == KW_CONTINUE) {
		NextNode();
		if (pcur->token != TK_SEMICOLON) {
			Row = pcur->row;
			longjmp(jmpbuf, CONTINUE_LACK_SEMICOLON);
		}
		NextNode();
		return;
	}
	else if (pcur->token == KW_BREAK) {
		NextNode();
		if (pcur->token != TK_SEMICOLON) {
			Row = pcur->row;
			longjmp(jmpbuf, BREAK_LACK_SEMICOLON);
		}
		NextNode();
		return;
	}
	else if (pcur->token == TK_END) {
		Row = pcur->row;
		longjmp(jmpbuf, LACK_ST);
	}
	else if (pcur->token == TK_SEMICOLON) {
		NextNode();
	}
	else if (pcur->token == TK_BEGIN) {
		st.push(pcur->s);
		NextNode();
	}
	else return;
}

void Return_Process() {
	if (FT == "void") {
		if (pcur->token == TK_SEMICOLON) {
			NextNode();
		}
		else {
			Row = pcur->row;
			longjmp(jmpbuf, RETURN_ERROR);
		}
	}
	else {
		if (pcur->token == TK_CINT || pcur->token == TK_IDENTV) {
			NextNode();
			if (pcur->token != TK_SEMICOLON) {
				Row = pcur->row;
				longjmp(jmpbuf, RETURN_ERROR);
			}
			NextNode();
		}
		else {
			Row = pcur->row;
			longjmp(jmpbuf, RETURN_ERROR);
		}
	}
}

void PrintBegin() {
	printf("|������������������������������������������������������������|\n");
	printf("|��������������������Virtual Compiler 6.0��������������������|\n");
	printf("|������������������������������������������������������������|\n");
}

void Primary_Assign() {
	E();
	if (pend == 1 && err == 0)
		return;
	else {
		Row = pcur->row;
		longjmp(jmpbuf, LACK_PRIMARY);
	}
}

void Next(){
	pcur++;
	if (pcur->s == ";"|| pcur->s == "," || pcur->s == ")")
		pend = 1;
}

void E(){
	C();
	while (pcur->token >= TK_EQ&& pcur->token <= TK_GEQ){
		Next();
		C();
	}
}

void C() {
	T(); 
	while (pcur->token == TK_PLUS || pcur->token == TK_MINUS || pcur->token == TK_ASSIGN){
		Next();
		T();
	}
}
 
void T(){
	F();
	while (pcur->token == TK_MUL || pcur->token == TK_DIVIDE){
		Next();
		F();
	}
}

void F(){
	if (pcur->token == TK_CINT || pcur->token== TK_IDENTV){
		Next();
	}
	else if (pcur->token == TK_OPENPA){
		Next();
		E();
		if (pcur->token == TK_CLOSEPA){
			Next();
		}
		else{	
			Row = pcur->row;
			err = -1;
		}
	}
	else{
		err = -1;
		Row = pcur->row;
		longjmp(jmpbuf, LACK_PRIMARY);
	}
}
