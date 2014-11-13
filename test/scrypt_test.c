#include <stdlib.h>

#include <libibur/test.h>
#include <libibur/util.h>

#include <ibcrypt/scrypt.h>

const static char* pass[] = {
	"",
	"password",
	"pleaseletmein",
	"pleaseletmein"
};

const static size_t plen[] = {
	0,
	8,
	13,
	13
};

const static char* salt[] = {
	"",
	"NaCl",
	"SodiumChloride",
	"SodiumChloride"
};

const static size_t slen[] = {
	0,
	4,
	14,
	14
};

const static uint64_t N[] = {
	16,
	1024,
	16384,
	1048576
};

const static uint32_t r[] = {
	1,
	8,
	8,
	8
};

const static uint32_t p[] = {
	1,
	16,
	1,
	1
};

const static char* out[] = {
	"77 d6 57 62 38 65 7b 20 3b 19 ca 42 c1 8a 04 97\
	 f1 6b 48 44 e3 07 4a e8 df df fa 3f ed e2 14 42\
	 fc d0 06 9d ed 09 48 f8 32 6a 75 3a 0f c8 1f 17\
	 e8 d3 e0 fb 2e 0d 36 28 cf 35 e2 0c 38 d1 89 06",
	"fd ba be 1c 9d 34 72 00 78 56 e7 19 0d 01 e9 fe\
	 7c 6a d7 cb c8 23 78 30 e7 73 76 63 4b 37 31 62\
	 2e af 30 d9 2e 22 a3 88 6f f1 09 27 9d 98 30 da\
	 c7 27 af b9 4a 83 ee 6d 83 60 cb df a2 cc 06 40",
	"70 23 bd cb 3a fd 73 48 46 1c 06 cd 81 fd 38 eb\
	 fd a8 fb ba 90 4f 8e 3e a9 b5 43 f6 54 5d a1 f2\
	 d5 43 29 55 61 3f 0f cf 62 d4 97 05 24 2a 9a f9\
	 e6 1e 85 dc 0d 65 1e 40 df cf 01 7b 45 57 58 87",
	"21 01 cb 9b 6a 51 1a ae ad db be 09 cf 70 f8 81\
	 ec 56 8d 57 4a 2f fd 4d ab e5 ee 98 20 ad aa 47\
	 8e 56 fd 8f 4b a5 d0 9f fa 1c 6d 92 7c 40 f4 c3\
	 37 30 40 49 e8 a9 52 fb cb f4 5c 6f a7 7a 41 a4"
};

const static size_t dkLen[] = {
	64,
	64,
	64,
	64
};

void scrypt_tests() {
	for(int i = 0; i < sizeof(dkLen)/sizeof(*dkLen); i++) {
		uint8_t* eout = malloc(dkLen[i]);
		uint8_t* aout = malloc(dkLen[i]);
		
		from_hex(out[i], eout);
		
		scrypt((uint8_t*)pass[i], plen[i], (uint8_t*)salt[i], slen[i], N[i], r[i], p[i], dkLen[i], aout);
		
		assert_equals(eout, aout, dkLen[i], "scrypt test");
		
		free(eout);
		free(aout);
	}
}
