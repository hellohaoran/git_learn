#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
int main()
{
	FILE* fp;
	fp = fopen("file.txt", "r");//���ļ�
	if (NULL == fp)
	{
		perror("fopen");//�жϸո�ִ��ʧ�ܵĺ�����ʧ��ԭ��
		return -1;
	}
	//��ȡ�ļ�,һ�ζ�һ���ַ�
	//char c;
	//while ((c = fgetc(fp)) != EOF)
	//{
	//	putchar(c);
	//}
	char buf[1000];//һ�ζ�һ��
	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		printf("%s", buf);
	}
	//�ر��ļ�
	fclose(fp);
	return 0;
}