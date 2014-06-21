#include <stdlib.h>
#include <stdint.h>

#include "bignum.h"
#include "bignum_util.h"

/* size is in 64 bit blocks */
int bnu_resize(BIGNUM* r, uint32_t size) {
	if(r->size == size) {
		return 0;
	}

	/* check for realloc fail */
	uint64_t* ptr = realloc(r->d, sizeof(uint64_t) * (uint64_t) size);
	if(ptr == NULL) {
		return 1;
	}
	r->d = ptr;

	/* zero allocated space */
	for(uint32_t i = r->size; i < size; i++) {
		r->d[i] = 0;
	}
	r->size = size;
	return 0;
}

int bnu_trim(BIGNUM* r) {
	if(r == NULL) {
		return -1;
	}

	if(r->size == 0) {
		return 0;
	}

	int64_t i = r->size; /* use signed so that we can go lower than 0 */
	do {
		i--;
	} while(r->d[i] == 0 && i >= 0);

	return bnu_resize(r, (uint32_t) (i + 1));
}

static inline char fhex(const uint8_t v) {
	if(v < 10) {
		return (char) (v + '0');
	} else {
		return (char) (v + 'a' - 10);
	}
}

int bnu_tstr(char* out, const BIGNUM* a) {
	uint32_t i = a->size;
	size_t pos = 0;

	if(a->neg) {
		out[0] = '-';
		pos++;
	}

	while(i--) {
		for(int j = 15; j >= 0; j--) {
			uint8_t val = (a->d[i] & (0xfULL << (j * 4))) >> (j * 4);
			out[pos] = fhex(val);
			pos++;
		}
	}
	out[pos] = '\0';

	return 0;
}