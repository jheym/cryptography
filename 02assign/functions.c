
// Write ptlen+12 bytes to ct (ciphertext followed by 12 byte random nonce)
void chat_encrypt(void *k, void *pt, int ptlen, void *ct) {

}

// Write ctlen-12 bytes to pt (using the last 12 bytes of ct as the nonce)
void chat_decrypt(void *k, void *ct, int ctlen, void *pt) {
}
