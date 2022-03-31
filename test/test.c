#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdio.h>
#include <string.h>

int main(){
	// Fill IV with random bits

	unsigned char iv[16];
	
	printf("%s\n", iv);	
	RAND_bytes(iv, 16);

	printf("%s\n", iv);	

}
