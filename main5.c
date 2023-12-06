#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mylib.h" //헤더를 통해 분할하여 함수 선언 

//void func(); //선언 --from [mylib.c]에서 
#define DEBUG //매크로  

int main(int argc, char *argv[]) {
	
	func();
	#ifdef DEBUG //매크로 있는지 검사 
		printf("디버그 출력\n");
	#endif
		
	return 0;
}
