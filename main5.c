#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mylib.h" //����� ���� �����Ͽ� �Լ� ���� 

//void func(); //���� --from [mylib.c]���� 
#define DEBUG //��ũ��  

int main(int argc, char *argv[]) {
	
	func();
	#ifdef DEBUG //��ũ�� �ִ��� �˻� 
		printf("����� ���\n");
	#endif
		
	return 0;
}
