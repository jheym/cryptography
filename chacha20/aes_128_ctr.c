#include <openssl/evp.h>

int main(){

	unsigned char pt[44] = "The quick brown fox jumps over the lazy dog";
	unsigned char ct[44];
	unsigned char pt2[44]; 
	unsigned char key[16] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6};	// Must be 128 bits for aes, normally needs to be random
	unsigned char iv[16] = {1,2,3,4,5,6,7,8,0,0,0,0,0,0,0,1};	// 16 byte buffer none followed by counter
	int len;
	int ct_bytes_written = 0;
	int pt_bytes_written = 0;
	
	/* Declare and initialize EVP context for handling EVP stuff */
	EVP_CIPHER_CTX *ctx; 
	ctx = EVP_CIPHER_CTX_new(); 

	/* Choosing the encryption algorithm
	int EVP_EncryptInit_ex(EVP_CIPHER_CTX *ctx, const EVP_CIPHER *type, ENGINE *impl, const unsigned char *key, const unsigned char *iv); */
	EVP_EncryptInit_ex(ctx, EVP_aes_128_ctr(), NULL, key, iv); // Key must be 128 bits long

	/* int EVP_EncryptUpdate(EVP_CIPHER_CTX *ctx, unsigned char *out, int *outl, const unsigned char *in, int inl); */
	EVP_EncryptUpdate(ctx, ct, &len, pt, 44);
	
	ct_bytes_written = ct_bytes_written + len;

	/* int EVP_EncryptFinal_ex(EVP_CIPHER_CTX *ctx, unsigned char *out, int *outl); */
	EVP_EncryptFinal_ex(ctx, ct + ct_bytes_written, &len);
	ct_bytes_written = ct_bytes_written + len;
	
	iv[15] = 1;
	EVP_DecryptInit_ex(ctx, EVP_aes_128_ctr(), NULL, key, iv);
	EVP_DecryptUpdate(ctx, pt2, &len, ct, 44);
	ct_bytes_written = ct_bytes_written + len;
	EVP_DecryptFinal_ex(ctx, pt2 + pt_bytes_written, &len);
	pt_bytes_written = pt_bytes_written + len;
	EVP_CIPHER_CTX_free(ctx);

	printf("%s\n", pt2);

	return 0;	
}

