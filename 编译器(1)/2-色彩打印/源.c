#include <Windows.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);//�ȵõ�һ�����.
	;
	SetConsoleTextAttribute(h, FOREGROUND_INTENSITY);//��ɫ
	printf("first\n");
	SetConsoleTextAttribute(h, FOREGROUND_GREEN | FOREGROUND_INTENSITY);//��ɫ
	printf("second\n");
	SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN);//��ɫ������
	printf("third\n");
	SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_BLUE);	//��ɫ
	printf("fourth\n");
	system("pause");
}