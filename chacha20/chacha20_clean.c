#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdio.h>
#include <string.h>

// Write ptlen+12 bytes to ct (ciphertext followed by 12 byte random nonce)
// Key is 32 bytes
void chat_encrypt(void *k, void *pt, int ptlen, void *ct) {

	// Print plaintext in hex
	unsigned char *plaintext = (unsigned char *)pt; 
	printf("Plaintext: ");
    for (int i = 0; i < ptlen; i++)
        putchar(plaintext[i]);
    printf("\n");
	// printf("plaintext: %s\n", (unsigned char *)pt);
	// printf("ptlen: %d\n", ptlen);
	// printf("key: %s\n", (unsigned char *)k);

	int len;
	int ct_bytes_written = 0;
	unsigned char iv[16];
	
	RAND_bytes(iv, 16);	
	memset(iv, 0, 4);
	
	EVP_CIPHER_CTX *ctx;
	ctx = EVP_CIPHER_CTX_new();
	
	EVP_EncryptInit_ex(ctx, EVP_chacha20(), NULL, (unsigned char *)k, iv);
	
	EVP_EncryptUpdate(ctx, (unsigned char *)ct, &len, (unsigned char *)pt, ptlen);
	ct_bytes_written = ct_bytes_written + len;
	
	EVP_EncryptFinal_ex(ctx, (unsigned char *)ct + ct_bytes_written, &len);
	ct_bytes_written = ct_bytes_written + len;

	memcpy(ct + ct_bytes_written, iv + 4, 12); 

	EVP_CIPHER_CTX_free(ctx);


	/*********** Debugging ***********
	// Print nonce from IV
	printf("IV Nonce: { ");
	for (unsigned int i = 4; i < sizeof(iv); i++) 
		printf("%x ", (unsigned int)iv[i]);
	printf("}\n");
	// Print nonce from ciphertext
	unsigned char ct_nonce[12];
	memcpy(ct_nonce, ct + ct_bytes_written, 12);
	printf("CT Nonce: { ");
	for (unsigned int i = 0; i < sizeof(ct_nonce); i++)
		printf("%x ", (unsigned int)ct_nonce[i]);
	printf("}\n");
	*********** End Debugging ***********/
}

// Write ctlen-12 bytes to pt (using the last 12 bytes of ct as the nonce)
void chat_decrypt(void *k, void *ct, int ctlen, void *pt) {
	
	int len;
	int pt_bytes_written = 0;
	unsigned char iv[16];

	memcpy(ct +  (ctlen - 12), iv + 4, 12); // Write last 12 bytes of ct to last 12 bytes of iv 
	memset(iv, 0, 4); // Setting first 4 bytes of iv to 0

	EVP_CIPHER_CTX *ctx;
	ctx = EVP_CIPHER_CTX_new();

	EVP_DecryptInit_ex(ctx, EVP_chacha20(), NULL, (unsigned char *)k, iv);
	
	EVP_DecryptUpdate(ctx, (unsigned char *)pt, &len, (unsigned char *)ct, ctlen - 12);
	pt_bytes_written = pt_bytes_written + len;

	EVP_DecryptFinal_ex(ctx, (unsigned char *)pt + pt_bytes_written, &len);
	pt_bytes_written = pt_bytes_written + len;

	EVP_CIPHER_CTX_free(ctx);
	
	// Print decrypted plaintext in hex
	printf("Decrypted: ");
	unsigned char *plaintext = (unsigned char *)pt; 
    for (int i = 0; i < (ctlen - 12); i++)
        putchar(plaintext[i]);
		// printf("%c ", (char *)pt + i);
    printf("\n");
	//	printf("%s\n", (unsigned char *)pt); // Print decrypted text for Debugging
}


int main (){

	unsigned char key[32] = "Welcome to hell-o world mwahaha";
	unsigned char plaintext[44] = "The quick brown fox jumps over the lazy dog";
	unsigned char ciphertext[56];
	
	chat_encrypt((void *)key, (void *)plaintext, 44, (void *)ciphertext);
	chat_decrypt((void *)key, (void *)ciphertext, 56, (void *)plaintext); 
}

