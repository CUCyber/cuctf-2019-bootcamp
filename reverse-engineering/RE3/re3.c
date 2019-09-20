#include <stdio.h>

#define OBFS(a) (a ^ 0x3b) + 0x10

#define DEOB(str) do {\
    char *ptr = str; \
    while (*ptr){ \
        *ptr -= 0x10; \
        *ptr ^= 0x3b; \
        *ptr++;\
    } \
} while(0)

void printFlag(){
	char str1[] = {
        OBFS('B'), OBFS('O'), OBFS('O'), OBFS('T'),
        OBFS('C'), OBFS('A'), OBFS('M'), OBFS('P'),
        OBFS('{'), OBFS('p'), OBFS('a'), OBFS('t'),
        OBFS('c'), OBFS('h'), OBFS('e'), OBFS('d'),
        OBFS('_'), OBFS('b'), OBFS('i'), OBFS('n'),
        OBFS('a'), OBFS('r'), OBFS('y'), OBFS('}'),
    };
	DEOB(str1);
	puts(str1);
}

int main() {
    int a = 0;

    if (a == 1){
        printFlag();
    }

    return 0;
}
