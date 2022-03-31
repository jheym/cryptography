#include <openssl/evp.h>
#include <openssl/rand.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
void encrypt_chat(void *k, void *j){

	

	printf("%s\n%s\n", (unsigned char *)k, (unsigned char *)j);
	

}

int main() {
	
	unsigned char key[5] = "1234"; 
	unsigned char jay[5] = "5678";

	encrypt_chat((void *)key, (void*)jay);

	unsigned char iv[16];
	RAND_bytes(iv, 16);

	for (unsigned int i = 0; i < sizeof(iv) - 1; i++)
		printf("%d", (unsigned int)iv[i]);

}
