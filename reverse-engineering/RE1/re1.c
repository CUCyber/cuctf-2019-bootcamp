#include <stdio.h>
#include <string.h>

int main(){
	char buffer[10];

	printf("What is the password?\n");
	fgets(buffer, 10, stdin);
	buffer[strcspn(buffer, "\r\n")] = 0;

	if (!strcmp(buffer, "the password is password")){
		printf("BOOTCAMP{welcome_to_reverse_engineering}\n");
	} else {
		printf("Password incorrect!\n");
	}

	return 0;
}
