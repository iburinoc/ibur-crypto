#include <stdio.h>

#include <libibur/util.h>

#include "sha256.h"

int main() {
	char* m = "a\n";
	//char* m = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456\n";
	uint8_t out[32];
	sha256((uint8_t*) m, 2, out);
	printbuf(out, 32);
}
