#include <openssl/evp.h>

int main() {
	char pt[44] = "The quick brown fox jumps over the lazy dog"; //Plaintext
	char ct[44]; // Ciphertext
	char pt2[44];
	unsigned char key[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6}; // Should be random, but not here for clarity
	unsigned char iv[16] = {1, 2, 3, 4, 5, 6, 7, 8, 0, 0, 0, 0, 0, 0, 0, 1}; // IV
	int len;
	int ct_bytes_written = 0;

	EVP_CIPHER_CTX *ctx;
	ctx = EVP_CIPHER_CTX_new();
	EVP_EncryptInit_ex(ctx, EVP_aes_128_ctr(), NULL, key, iv);
	EVP_EncryptUpdate(ctx, ct, &len, pt, 44); // EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
	EVP_EncryptFinal_ex(ctx, ct + ct_bytes_written, &len);
	ct_bytes_written = ct_bytes_written + len;
	EVP_CIPHER_CTX_free(ctx); // Release AND zeroes the memory

	return 0;
}
