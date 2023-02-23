#include <openssl/evp.h>

void chat_encrypt(void *k, void *pt, int ptlen, void *ct) {
         
	unsigned char plaintext = pt;
	unsigned char ciphertext = ct;
	unsigned char key = k;
	int plaintext_len = ptlen;
	unsigned char iv[16] = {1,2,3,4,5,6,7,8,0,0,0,0,0,0,0,1};

	EVP_CIPHER_CTX *ctx;     
	ctx = EVP_CIPHER_CTX_new();
	EVP_EncryptInit_ex(ctx, EVP_chacha20(), NULL, key, iv);
}

int main(){

	unsigned char pt[44] = "The quick brown fox jumps over the lazy dog";
	unsigned char ct[56]; // ptlen + twelve bytes
	unsigned char key[16] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6};

	
	
	chat_encrypt(&key, &pt, 44, &pt);  

}
