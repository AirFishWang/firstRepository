//#include <stdio.h>
//#include <string.h>
#include <iostream>
using namespace std;


int main()
{
	//char a[3] = {'e'};
	//std::cout << strlen(a) << std::endl;  //���㵽NULL֮ǰ�� �����ǲ�ȷ���ĳ���


	//char src[] = "ag";
	//char dst[] = "abc";
	//strcpy(dst, src);            //���д���

 //   char *p = "hello world";
 //   //p[0] = 'k';  //�����г���

	////char src[] = "ag";
	////char dst[] = "abc";
	////strcpy(dst, src);            //���д���

 //   string file = "C:/Users/wangchun/Desktop/1.png";
 //   int pos = file.find_last_of("/\\");
 //   cout << file.substr(pos + 1).c_str() << endl;

    const char* a = "Hello world";
    string str(a);
    cout << str.c_str() << endl;

	return 0;
}