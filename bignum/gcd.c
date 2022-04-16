#include <openssl/bn.h>

int main(int argc, char *argv[]){
    if (argc != 3){
        printf("There should be two args\n");
        return(EXIT_FAILURE);
    }
    BIGNUM *a = BN_new();
    BIGNUM *b = BN_new();
    BIGNUM *tmp = BN_new();
    BN_CTX *ctx = BN_CTX_new();
    BN_dec2bn(&a, argv[1]);
    BN_dec2bn(&b, argv[2]);
    while (!(BN_is_zero(b))){
        if (BN_cmp(a, b) > 0)
            BN_swap(a, b);
        BN_mod(tmp, b, a, ctx);
        BN_swap(b, tmp);
        BN_clear(tmp);
    }
   
    char *string = BN_bn2dec(a);
    printf("%s\n", string);
    BN_clear_free(a);
    BN_clear_free(b);
    BN_clear_free(tmp);
    BN_CTX_free(ctx);
    free(string);
}
