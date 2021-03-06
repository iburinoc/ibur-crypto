#ifndef IBCRYPT_CHACHA_H
#define IBCRYPT_CHACHA_H

//#define CHACHA_DEBUG

#include <stdint.h>

typedef struct {
	uint64_t nonce;
	uint64_t count;
	uint8_t key[32];
	int ksize;
	uint8_t stream[64];
} CHACHA_CTX;

/* the chacha core hash function
 * in and out can overlap */
void chacha_core(const uint8_t in[64], uint8_t out[64]);

/* the chachaexpansion function 
 * ksize must be 16 or 32, otherwise
 * this function will fail silently */
void chacha_expand(const uint8_t *const k, const int ksize, const uint8_t n[16], uint8_t out[64]);

/* initialize a chacha context
 * ksize is in bytes */
void chacha_init(CHACHA_CTX *ctx, const uint8_t *key, const int ksize, const uint64_t nonce);

/* encrypt/decrypt a section */
void chacha_stream(CHACHA_CTX *ctx, const uint8_t *const in, uint8_t *const out, const uint64_t len);

/* zeroes an initialized chacha context */
void chacha_final(CHACHA_CTX *ctx);

/* convenience functions */
void chacha_enc(const uint8_t *key, const int ksize, const uint64_t nonce, const uint8_t *const in, uint8_t *const out, const uint64_t len);
void chacha_dec(const uint8_t *key, const int ksize, const uint64_t nonce, const uint8_t *const in, uint8_t *const out, const uint64_t len);

#endif
