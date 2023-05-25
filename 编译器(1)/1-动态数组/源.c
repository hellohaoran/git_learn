#include <stdio.h>
#include<stdlib.h>
//动态数组实现原理
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
	a = (int*)realloc(a, sizeof(int) * 10); // realloc() 可以将原先的数组 赋值到新的数组 ;并将数组扩容; 

	return 0;
}