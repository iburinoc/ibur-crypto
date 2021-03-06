#include <stdlib.h>

#include "bignum.h"
#include "bignum_util.h"

// don't resize, do inplace
int rmod_words(uint64_t *r, const uint32_t rlen, const bignum *n) {
	bignum nt = BN_ZERO;

	if(bni_cpy(&nt, n) != 0 || bnu_resize(&nt, rlen+1)) {
		return 1;
	}

	uint64_t *const nd = nt.d;

	uint64_t shift = 0;
	/* shift nt so that its greater than a */
	while(cmp_words(nd, nt.size, r, rlen) <= 0) {
		lshift_words(nd, nd, n->size + (shift + 32) / 64, 32);
		shift += 32;
	}

	for(uint64_t i = 0; i < shift; i++) {
		rshift_words(nd, nd, nt.size-(i/64), 1);
		if(cmp_words(nd, nt.size-(i/64), r, rlen) <= 0) {
			sub_words(r, r, rlen, nd, nt.size-(i/64));
		}
	}

	return 0;
}

int bno_rmod(bignum *r, const bignum *a, const bignum *n) {
	if(r == NULL || a == NULL || n == NULL) {
		return -1;
	}

	if(bno_cmp(a, n) < 0) {
		return bni_cpy(r, a);
	}

	bignum nt = BN_ZERO;
	bignum at = BN_ZERO;
	if(bni_cpy(&nt, n) != 0 || bni_cpy(&at, a) != 0) {
		return 1;
	}

	uint64_t shift = 0;
	/* shift nt so that its greater than a */
	while(bno_cmp(&nt, &at) <= 0) {
		if(bno_lshift(&nt, &nt, 32) != 0) {
			return 1;
		}
		shift += 32;
	}

	for(uint64_t i = 0; i < shift; i++) {
		rshift_words(nt.d, nt.d, nt.size-(i/64), 1);
		if(cmp_words(nt.d, nt.size-(i/64), at.d, at.size) <= 0) {
			sub_words(at.d, at.d, at.size, nt.d, nt.size-(i/64));
		}
	}

	if(bni_cpy(r, &at) != 0) {
		return 1;
	}

	bnu_free(&nt);
	bnu_free(&at);

	return bnu_trim(r);
}

/* return n - a */
int bno_neg_mod(bignum *r, const bignum *a, const bignum *n) {
	if(bno_rmod(r, a, n) != 0) {
		return 1;
	}
	return bno_sub(r, n, r);
}

// note: if the GCD of a and n isn't 1, the execution is undefined
int bno_inv_mod(bignum *inv, const bignum *_a, const bignum *_n) {
	/* compute the inverse using the extended euclidean algorithm */
	bignum t = BN_ZERO, newt = BN_ZERO;
	bignum r = BN_ZERO, newr = BN_ZERO;

	bignum tmp = BN_ZERO, tmp2 = BN_ZERO, quot = BN_ZERO, remain = BN_ZERO;

	if(bni_int(&newt, 1) != 0 || bni_cpy(&r, _n) != 0 || bni_cpy(&newr, _a) != 0) {
		return 1;
	}

	bno_rmod(&newr, &newr, &r);

	while(newr.size != 0) {
		if(bno_div_mod(&quot, &remain, &r, &newr) != 0) {
			return 1;
		}

		// calculate (t, newt) := (newt, t - quotient * newt)
		{
			if(bno_mul_mod(&tmp, &quot, &newt, _n) != 0 || bno_neg_mod(&tmp, &tmp, _n) != 0) {
				return 1;
			}

			tmp2 = newt;
			newt = BN_ZERO;

			if(bno_add_mod(&newt, &t, &tmp, _n) != 0) {
				return 1;
			}

			bnu_free(&t);
			t = tmp2;
		}

		// calculate (r, newr) := (newr, r - quotient * newr)
		{
			bnu_free(&r);
			r = newr;
			newr = remain;
			remain = BN_ZERO;
		}
	}

	if(bnu_free(inv) != 0) {
		return 1;
	}
	*inv = t;
	return bnu_free(&newt) || bnu_free(&r) || bnu_free(&newr) || bnu_free(&tmp)
		|| bnu_free(&quot);
}
