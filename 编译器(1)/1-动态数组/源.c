#include <stdio.h>
#include<stdlib.h>
//��̬����ʵ��ԭ��
//typedef struct DynArray {
//	int count;
//	int capacity;
//	void** data;
//}DynArray;
int main()
{
	int* a = (int*)malloc(sizeof(int) * 5);
	int i =0 ;
	for (; i < 5; i++)
	{
 		a[i] = i;
	}
	a = (int*)realloc(a, sizeof(int) * 10); // realloc() ���Խ�ԭ�ȵ����� ��ֵ���µ����� ;������������; 

	return 0;
}