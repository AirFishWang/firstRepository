//#include <stdio.h>
//#include <string.h>
#include <iostream>
//using namespace std;


int main()
{
	char a[3] = {'e'};
	std::cout << strlen(a) << std::endl;  //���㵽NULL֮ǰ�� �����ǲ�ȷ���ĳ���

	char src[] = "abcdefg";
	char *dst = "abc";
	strcpy(dst, src);            //���д���
	return 0;
}