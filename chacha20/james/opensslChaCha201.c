#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <string.h>

// Write ptlen+12 bytes to ct (ciphertext followed by 12 byte random nonce)
void chat_encrypt(void *k, void *pt, int ptlen, void *ct)
{
	// change void to proper type
	uint8_t *k8 = (uint8_t *)k;
	uint8_t *pt8 = (uint8_t *)pt;
	uint8_t *ct8 = (uint8_t *)ct;
	int len; // variable to hold the length
	int ct_bytes_written = 0; // keeps track of bytes written so the nonce can be added to the end
	

	uint8_t iv[16] = {00, 00, 00, 00}; // set the first 4 bytes of the iv to the counter
	RAND_bytes(iv + 4, 12); // Set the last 12 bytes of iv to the random nonce

	EVP_CIPHER_CTX *ctx; // context variable declaration
	ctx = EVP_CIPHER_CTX_new(); // intialize context with new constructor

	EVP_EncryptInit_ex(ctx, EVP_chacha20(), NULL, k8, iv); // intit the encryption

	EVP_EncryptUpdate(ctx, ct8, &len, pt8, ptlen); // start the encryption process, in this case only 1 round needed
	ct_bytes_written = len; // keep track of the length of the bytes written

	EVP_EncryptFinal_ex(ctx, ct8+len, &len); // last step in encryption (no s box)

	ct_bytes_written = ct_bytes_written + len; // keep track of the length of the bytes written

	/*// Add the nonce to the end of the cipher text
	memset(iv+5, ct8[ct_bytes_written- 11], 1);
	memset(iv+6, ct8[ct_bytes_written - 10], 1);
	memset(iv+7, ct8[ct_bytes_written - 9], 1);
	memset(iv+8, ct8[ct_bytes_written - 8], 1);
	memset(iv+9, ct8[ct_bytes_written - 7], 1);
	memset(iv+10, ct8[ct_bytes_written - 6], 1);
	memset(iv+11, ct8[ct_bytes_written - 5], 1);
	memset(iv+12, ct8[ct_bytes_written - 4], 1);
	memset(iv+13, ct8[ct_bytes_written - 3], 1);
	memset(iv+14, ct8[ct_bytes_written - 2], 1);
	memset(iv+15, ct8[ct_bytes_written - 1], 1);
	memset(iv+16, ct8[ct_bytes_written], 1);
	*/

	memcpy((void *)ct + (ct_bytes_written - 12), iv + 4, 12); 

	// Free memory, zero sensitive stack elements
	EVP_CIPHER_CTX_free(ctx);

};

void chat_decrypt(void *k, void *ct, int ctlen, void *pt)
{
	uint8_t *k8 = (uint8_t *)k;
	uint8_t *ct8 = (uint8_t *)pt;
	uint8_t *pt8 = (uint8_t *)ct;
	int len; // variable to hold the length
	int pt_bytes_written = 0; // keeps track of bytes written so the nonce can be added to the end

	uint8_t iv[16] = {00, 00, 00, 00}; // reset the counter back to 0
	
	/*
	// Add the nonce to the end of the cipher text
	memset(iv+5, ct8[ctlen - 11], 1);
	memset(iv+6, ct8[ctlen - 10], 1);
	memset(iv+7, ct8[ctlen - 9], 1);
	memset(iv+8, ct8[ctlen - 8], 1);
	memset(iv+9, ct8[ctlen - 7], 1);
	memset(iv+10, ct8[ctlen - 6], 1);
	memset(iv+11, ct8[ctlen - 5], 1);
	memset(iv+12, ct8[ctlen - 4], 1);
	memset(iv+13, ct8[ctlen - 3], 1);
	memset(iv+14, ct8[ctlen - 2], 1);
	memset(iv+15, ct8[ctlen - 1], 1);
	memset(iv+16, ct8[ctlen], 1);
	*/

	memcpy(iv + 4, (void *)ct + (ctlen - 12), 12); 

	EVP_CIPHER_CTX *ctx; // context variable declaration
	ctx = EVP_CIPHER_CTX_new(); // intialize context with new constructor

	EVP_DecryptInit_ex(ctx, EVP_chacha20(), NULL, k8, iv); // Init the decryption process

	EVP_DecryptUpdate(ctx, pt8, &len, ct8, ctlen - 12); // first round of decryption
    pt_bytes_written = pt_bytes_written + len; // keep track of pt bytes written

    EVP_DecryptFinal_ex(ctx, pt8 + len, &len); // final round of dycrpt does not have an s box
    pt_bytes_written = pt_bytes_written + len; // keep track of pt bytes written

    EVP_CIPHER_CTX_free(ctx); // securely clean up function

    printf("%s\n", pt8);
};


int main() {

	uint8_t pt[44] = "The quick brown fox jumps over the lazy dog"; 
	uint8_t ct[56];
	int ctlen = 56;
	int len; // 

	uint8_t key[4]; // 32 byte long key
	RAND_bytes(key, 4); // init key with random data

	chat_encrypt(key, pt, 44, ct); 

	chat_decrypt(key, ct, ctlen, pt);

	return 0;
};
