#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

#define BLK_LEN 16
#define BUF_LEN 512

// NOTE!! For clarity's sake, this example has no error checking.
//        In real code you need to test for errors and handle them.

int main(int argc, const char* argv[]) {
    int len, bytes_read;
    char passphrase[256];
    unsigned char key[32]; // Receive SHA-2-256 hash of user pass phrase
    unsigned char iv[BLK_LEN];
    unsigned char in_buf[BUF_LEN];
    unsigned char out_buf[BUF_LEN+BLK_LEN];
    EVP_MD_CTX *mdctx;

    if (argc != 3) {
        printf("Error: Need two arguments, src and dst files\n");
        return EXIT_FAILURE;
    }

    // Open files to read and write
    FILE *src = fopen(argv[1], "r");
    FILE *dst = fopen(argv[2], "w");
    if (src==NULL || dst==NULL) {
        printf("Error: could not open file(s)\n");
        return EXIT_FAILURE;
    }
    
    // Get passphrase for encryption
    printf("Enter passphrase up to 255 characters: ");
    fgets(passphrase, 256, stdin);
    
    // Hash passphrase, put 256 bit result in "key"
    mdctx = EVP_MD_CTX_create();
    EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);
    EVP_DigestUpdate(mdctx, passphrase, strlen(passphrase));
    EVP_DigestFinal_ex(mdctx, key, NULL);
    EVP_MD_CTX_destroy(mdctx);
        
    // Write a random IV to dst
    FILE *rand = fopen("/dev/urandom", "r");
    fread(iv, 1, BLK_LEN, rand);
    fwrite(iv, 1, BLK_LEN, dst);
    fclose(rand);

    // Set up context with alg, key and iv
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    bytes_read = fread(in_buf, 1, BUF_LEN, src);  
    while (bytes_read > 0) {
        EVP_EncryptUpdate(ctx, out_buf, &len, in_buf, bytes_read);
        fwrite(out_buf, 1, len, dst);
        bytes_read = fread(in_buf, 1, BUF_LEN, src);  
    }
    EVP_EncryptFinal_ex(ctx, out_buf, &len);
    fwrite(out_buf, 1, len, dst);
        
    // Free memory, zero sensitive stack elements
    EVP_CIPHER_CTX_free(ctx);
    OPENSSL_cleanse(key, sizeof(key));
    OPENSSL_cleanse(in_buf, sizeof(in_buf));
    OPENSSL_cleanse(passphrase, sizeof(passphrase));
    fclose(src);
    fclose(dst);
    
    return EXIT_SUCCESS;
}