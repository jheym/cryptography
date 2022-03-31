#include <stdio.h>
#include <openssl/evp.h>

int main(int argc, char *argv[]) {
    unsigned char digest[32];
    FILE *fp = fopen(argv[1], "r");
    EVP_MD_CTX *mdctx;
    /* Initialize buffer to file size and read into buf */
    fseek(fp, 0L, SEEK_END);    // Place fp at EOF
    int fsz = ftell(fp);        // Store index of EOF position
    char buf[fsz];              // Init buf to file size
    fseek(fp, 0L, SEEK_SET);    // Set fp back to beginning of file
    fread(buf, 1, fsz, fp);
    /* OpenSSL voodoo */
    mdctx = EVP_MD_CTX_create();
    EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);
    EVP_DigestUpdate(mdctx, buf, fsz);
    EVP_DigestFinal_ex(mdctx, digest, NULL);
    EVP_MD_CTX_destroy(mdctx);
    /* Print sum in two-character lowercase hex*/
    for (int i = 0; i < 32; i++)
        printf("%02x", (unsigned int)digest[i]);   
    printf("\n");
    return(EXIT_SUCCESS);
}