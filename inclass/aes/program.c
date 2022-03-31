#include <openssl/evp.h>
#include <string.h>
#include <stdio.h>

/* Pseudocode

	CTR-AES-128(in, out, bytes, nonce, rdkeys)
		setup blocks
		while bytes > 0
			aes64(blockin, blockout)
			todo min(bytes, 64)
			xor todo bytes of in with blockout
			bytes -= todo
			in += todo
			out += todo
			
			 


*/		

void ctr(void *in, void *out, int bytes, void *n, void *rdkey){
	uint8_t blkin[64], blkout[64];
	uint8_t *inp = in;
	uint8_t *outp = out;

	memset(blkin, 0, 64); // Fills block with 64 bytes of 0
	memcpy(blkin, n, 8);
	memcpy(blkin, n+16, 8);
	memcpy(blkin, n+32, 8);
	memcpy(blkin, n+48, 8); // Places nonce at respective block starts
	blkin[31] = 1;
	blkin[47] = 2;
	blkin[63] = 3;

	while (bytes > 0){
		aes64(blkin, blkout, rdkey); // Refer to prev aes lecture video for the code for this function
		int todo = (bytes < 64 ? bytes : 64); // This is to get min... TRINARY OPERATOR... look it up
		for (int i=0; i<todo; i++)
			outp[i] = inp[i] ^ blkout[i];
		bytes -= todo;
		inp += todo;
		outp += todo; // Advancing the pointers to point where the next ciphertext should be written
		
		/* Load big endian, add 4, write big endian... use gcc builtin to reverse the order */
		/* builtin_bswap64 */
	}
	
	void inc64(void *p) {
	uint64_t *ptr = p;
	uint64_t x = __builtin_bswap64(*ptr); // Add the include for this
	x = __builtin_bswap64(x+4);
	*ptr = x;
	}

}





int main (){
}
