#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern void my_encrypt( char *input, char *output, char *key, unsigned int encrypt_switch) ;

void main()
{
	char *input = "this is a test string";
	char *key = "key.";

	printf("string: %s \nkey: %s\n",input,key);

	char *out1 = malloc(sizeof(char)*strlen(input)+1);

	char *out2 = malloc(sizeof(char)*strlen(input)+1);

	my_encrypt(input, out1, key, (unsigned int)0);

	printf("Encrypted string: %s\n", out1);

	my_encrypt(out1, out2, key, (unsigned int)1);

	printf("Decrypted string: %s\n", out2);

    free(out1);
    free(out2);

}
