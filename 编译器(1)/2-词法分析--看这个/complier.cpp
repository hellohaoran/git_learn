#include "complier.h"
//��ϣ����
int elf_hash(string key){
	int h = 0, g,i=0;
	while (key[i]){
		h = (h << 4) + key[i++];
		g = h & 0xf0000000;
		if (g)
			h ^= g >> 24;
		h &= ~g;
	}
	return h % MAXKEY;
}
//����tokenֵ��ӡ��ɫ
void printColor(string str,tokencode token){
	HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE); // windows�Դ��ĺ���;
	// ����token������ɫ 
	if(token>=TK_IDENT){//�����������ߺ�����Ϊ��ɫ 
		SetConsoleTextAttribute(h,FOREGROUND_INTENSITY);
	}else if(token>=KW_CHAR){//�ؼ���Ϊ��ɫ
		SetConsoleTextAttribute(h,FOREGROUND_GREEN|FOREGROUND_INTENSITY);
	}else if(token>=TK_CINT){//���ͳ����������ͳ������ַ��������ַ���������Ϊ��ɫ
		SetConsoleTextAttribute(h,FOREGROUND_RED|FOREGROUND_GREEN);
	}else{//�����Ϊ��ɫ
		SetConsoleTextAttribute(h,FOREGROUND_RED|FOREGROUND_INTENSITY);
	}
	if(-1==str[0]){// �������,����ɫ�ı����
		printf("\n ENd Of File");
		SetConsoleTextAttribute(h,FOREGROUND_RED|FOREGROUND_INTENSITY|FOREGROUND_BLUE|FOREGROUND_INTENSITY);
	}else{
		cout<<str; // ��ӡ�ַ�����ʱ�Ѿ�������ɫ
	}
}
//��ʼ���õ���ϣ��
//ʹ�� �õ��ַ�������Կ����ҵ�����token
void InitKeywords(pTkWord keywords,int keywordsLen,pTkWord *tk_Hashtable){ 
	for(int i=0;i<keywordsLen-1;++i){  // hash���ŵ���ָ��
		//tk_Hashtable[elf_hash(keywords[i].spelling)]=&keywords[i];  // �� pTKWord �ṹ��ָ��ĸ�ֵ;
		int idx = elf_hash(keywords[i].spelling);
		pTkWord curr = tk_Hashtable[idx];
		while (curr != NULL) {
			if (curr->spelling == keywords[i].spelling) {
				curr->tkcode = keywords[i].tkcode;

			}
			curr = curr->next;
		}
		// curr == NULL;
		(&keywords[i])->next=  tk_Hashtable[idx] ;
		tk_Hashtable[idx] = &keywords[i];
	}// hash��ͻûд;
	
}
//�ָ�һ���ַ�������ÿһ���ʴ浽word������һ�����飬������ÿ��Ԫ�ش�һ���ַ�������
int split(vector<string> &word,const string str){
	for (int i = 0; i < str.length(); i++) {
		if (isalpha(str[i]) || str[i] == '_') {//��������������Ӣ����ĸ���»��߿�ͷ
			string temp;
			while (isalnum(str[i]) || str[i] == '_') {//�������п���ΪӢ����ĸ�����ֺ��»���
				temp.push_back(str[i]); // �ַ���Ҳ����push_back
				i++;
			}
			word.push_back(temp);
			--i;// 
		}
		else if (str[i] == 34) {//�ַ�����������"һֱ������һ��"
			string temp;
			temp.push_back(str[i]);
			i++;
			while (str[i] != 34) {
				temp.push_back(str[i]);
				i++;
			}
			temp.push_back(str[i]);
			word.push_back(temp);
		}
		else if (str[i] == 39) {//�ַ�����.һ���Զ���'X'�����ַ�
			string temp;
			int n = 3;
			while (n--) {
				temp.push_back(str[i]);
				i++;
			}
			word.push_back(temp);
			// ispunct�Ȳ�����ĸҲ��������, �ո�, ����true, ||  
		}
		else if (ispunct(str[i]) || str[i] == ' ') {//����ɴ�ӡ���Ϳո�
			string temp;
			temp.push_back(str[i]);
			word.push_back(temp);
		}
		else if (isdigit(str[i])) {//�����֣����ͺ͸���
			string temp;
			while (isdigit(str[i])) {
				temp.push_back(str[i]);
				++i;
			}
			if (str[i] == '.') {
				++i;
				if (isdigit(str[i])) {
					temp.push_back('.');
					while (isdigit(str[i])) {
						temp.push_back(str[i]);
						i++;
					}
				}
				else {
					return -1;//�����ܲ�Ҫд����������ְ�
				}
			}
			word.push_back(temp);
			--i;
		}
		else if (str[i] == '\t') {//tab�����ĸ��ո����
			string temp = "    ";
			word.push_back(temp);
		}
		else if (str[i] == '\\' ){
			while (str[i] != ' ') {
				++i;
				}
	}
	}
}


pTkWord find_(string s, pTkWord* tk_Hashtable) {
	int idx = elf_hash(s);
	pTkWord curr = tk_Hashtable[idx];
	while (curr!=NULL)
	{
		if (s == curr->spelling) {
			return curr;
			
		}
		curr = curr->next;

	}
	return NULL;
	

}