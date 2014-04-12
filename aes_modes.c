#include <string.h>

#include "aes_modes.h"
#include "aes.h"
#include "util.h"

int encrypt_cbc_AES(const uint8_t* const message, const uint32_t length, const uint8_t* const iv, const AES_KEY* const key, uint8_t* const out) {
	if(length % 16 != 0) {
		return -1; // must be of proper size
	}
	
	uint8_t prev[16];
	memcpy(prev, iv, 16); // set up init vector
	
	uint8_t encbuf[16];
	
	for(uint32_t i = 0; i < length / 16; i++) {
		xor_bytes(message + i * 16, prev, 16, encbuf); // xor in iv
		encrypt_block_AES(encbuf, prev, key); // encrypt block
		memcpy(out + i * 16, prev, 16); // copy to output
	}
	return 0;
}

int decrypt_cbc_AES(const uint8_t* const message, const uint32_t length, const uint8_t* const iv, const AES_KEY* const key, uint8_t* const out) {
	if(length % 16 != 0) {
		return -1; // must be of proper size
	}
	
	uint8_t prev[16];
	memcpy(prev, iv, 16); // set up init vector
	
	uint8_t decbuf[16];
	
	for(uint32_t i = 0; i < length / 16; i++) {
		decrypt_block_AES(message + i * 16, decbuf, key); // decrypt block
		xor_bytes(decbuf, prev, 16, out + i * 16); // write to output
		memcpy(prev, message + i * 16, 16); // copy ciphertext for next prev
	}
	
	return 0;
}
