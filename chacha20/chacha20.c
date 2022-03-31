#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdio.h>
#include <string.h>



// Write ptlen+12 bytes to ct (ciphertext followed by 12 byte random nonce)
// Key is 32 bytes
void chat_encrypt(void *k, void *pt, int ptlen, void *ct) {

	// int ctlen = ptlen + 12; 
	int len;
	int ct_bytes_written = 0;

	unsigned char iv[16];
	
	/* Fills iv with 16 random bytes */
	RAND_bytes(iv, 16);	

	/* Zeroes first 4 bytes of iv */
	memset(iv, 0, 4);
	
	EVP_CIPHER_CTX *ctx;
	ctx = EVP_CIPHER_CTX_new();

	// int EVP_EncryptInit_ex(EVP_CIPHER_CTX *ctx, const EVP_CIPHER *type, ENGINE *impl, const unsigned char *key, const unsigned char *iv);	
	EVP_EncryptInit_ex(ctx, EVP_chacha20(), NULL, (unsigned char *)k, iv);

	// int EVP_EncryptUpdate(EVP_CIPHER_CTX *ctx, unsigned char *out, int *outl, const unsigned char *in, int inl);
	EVP_EncryptUpdate(ctx, (unsigned char *)ct, &len, (unsigned char *)pt, ptlen);
	
	// printf("ctlen after first update: %d/n", ctlen);

	ct_bytes_written = ct_bytes_written + len;

	// int EVP_EncryptFinal_ex(EVP_CIPHER_CTX *ctx, unsigned char *out, int *outl);
	EVP_EncryptFinal_ex(ctx, (unsigned char *)ct + ct_bytes_written, &len);

	ct_bytes_written = ct_bytes_written + len;

	memcpy(ct + ct_bytes_written, iv + 4, 12); 

	EVP_CIPHER_CTX_free(ctx);
}

// Write ctlen-12 bytes to pt (using the last 12 bytes of ct as the nonce)
void chat_decrypt(void *k, void *ct, int ctlen, void *pt) {
	
	// int ptlen = ctlen - 12; 
	int len;
	int pt_bytes_written = 0;
	unsigned char iv[16];

	// printf("ct before decrypt: %s\n", (unsigned char *)ct); // Debugging
	
	memcpy(ct +  (ctlen - 12), iv + 4, 12); // Write last 12 bytes of ct to last 12 bytes of iv 
	memset(iv, 0, 4); // Setting first 4 bytes of iv to 0


	EVP_CIPHER_CTX *ctx;
	ctx = EVP_CIPHER_CTX_new();

	// int EVP_DecryptInit_ex(EVP_CIPHER_CTX *ctx, const EVP_CIPHER *type, ENGINE *impl, const unsigned char *key, const unsigned char *iv);
	EVP_DecryptInit_ex(ctx, EVP_chacha20(), NULL, (unsigned char *)k, iv);

	// int EVP_DecryptUpdate(EVP_CIPHER_CTX *ctx, unsigned char *out, int *outl, const unsigned char *in, int inl);
	EVP_DecryptUpdate(ctx, (unsigned char *)pt, &len, (const unsigned char *)ct, ctlen);

	pt_bytes_written = pt_bytes_written + len;

	// int EVP_DecryptFinal_ex(EVP_CIPHER_CTX *ctx, unsigned char *outm, int *outl);
	EVP_DecryptFinal_ex(ctx, (unsigned char *)pt + pt_bytes_written, &len);
	
	pt_bytes_written = pt_bytes_written + len;

	EVP_CIPHER_CTX_free(ctx);

	printf("%s\n", (unsigned char *)pt); // Debugging
}


int main (){

	unsigned char key[32] = "Welcome to hell-o world mwahahah";
	unsigned char plaintext[44] = "The quick brown fox jumps over the lazy dog";
	unsigned char ciphertext[56];
	// int ptlen = 44;
	// int ctlen = 56;
	
	chat_encrypt((void *)key, (void *)plaintext, 44, (void *)ciphertext);

	chat_decrypt((void *)key, (void *)ciphertext, 56, (void *)plaintext); 

//	printf("%s\n", plaintext);
}

