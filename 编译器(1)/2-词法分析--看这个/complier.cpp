#include "complier.h"
//哈希处理
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
//根据token值打印颜色
void printColor(string str,tokencode token){
	HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE); // windows自带的函数;
	// 根据token设置颜色 
	if(token>=TK_IDENT){//变量名，或者函数名为灰色 
		SetConsoleTextAttribute(h,FOREGROUND_INTENSITY);
	}else if(token>=KW_CHAR){//关键字为绿色
		SetConsoleTextAttribute(h,FOREGROUND_GREEN|FOREGROUND_INTENSITY);
	}else if(token>=TK_CINT){//整型常量、浮点型常量、字符常量、字符串常量等为褐色
		SetConsoleTextAttribute(h,FOREGROUND_RED|FOREGROUND_GREEN);
	}else{//运算符为红色
		SetConsoleTextAttribute(h,FOREGROUND_RED|FOREGROUND_INTENSITY);
	}
	if(-1==str[0]){// 读到最后,将颜色改变回来
		printf("\n ENd Of File");
		SetConsoleTextAttribute(h,FOREGROUND_RED|FOREGROUND_INTENSITY|FOREGROUND_BLUE|FOREGROUND_INTENSITY);
	}else{
		cout<<str; // 打印字符串此时已经有了颜色
	}
}
//初始化得到哈希表
//使得 得到字符串后可以快速找到它的token
void InitKeywords(pTkWord keywords,int keywordsLen,pTkWord *tk_Hashtable){ 
	for(int i=0;i<keywordsLen-1;++i){  // hash表存放的是指针
		//tk_Hashtable[elf_hash(keywords[i].spelling)]=&keywords[i];  // 是 pTKWord 结构体指针的赋值;
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
	}// hash冲突没写;
	
}
//分割一行字符串，把每一个词存到word（看成一个数组，数组中每个元素存一个字符串）里
int split(vector<string> &word,const string str){
	for (int i = 0; i < str.length(); i++) {
		if (isalpha(str[i]) || str[i] == '_') {//变量名，必须以英文字母和下划线开头
			string temp;
			while (isalnum(str[i]) || str[i] == '_') {//变量名中可以为英文字母、数字和下划线
				temp.push_back(str[i]); // 字符串也可以push_back
				i++;
			}
			word.push_back(temp);
			--i;// 
		}
		else if (str[i] == 34) {//字符串常量，从"一直读到下一个"
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
		else if (str[i] == 39) {//字符常量.一次性读入'X'三个字符
			string temp;
			int n = 3;
			while (n--) {
				temp.push_back(str[i]);
				i++;
			}
			word.push_back(temp);
			// ispunct既不是字母也不是数字, 空格, 返回true, ||  
		}
		else if (ispunct(str[i]) || str[i] == ' ') {//其余可打印标点和空格
			string temp;
			temp.push_back(str[i]);
			word.push_back(temp);
		}
		else if (isdigit(str[i])) {//纯数字，整型和浮点
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
					return -1;//尽可能不要写出错误地数字吧
				}
			}
			word.push_back(temp);
			--i;
		}
		else if (str[i] == '\t') {//tab键用四个空格代替
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