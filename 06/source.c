
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int checkBuf(char* buf, int sz) {
	int i, j;
	for (i = 0; i < sz; ++i) {
		for (j = i + 1; j < sz; ++j) {
			if (buf[i] == buf[j]) {
				return i + 1; // dup after chars
			}
		}
	}
	return 0; // no dup, good buf
}

int pushBuf(char* buf, int j, int sz) {
	memcpy(buf, buf + j, sz - j);
	return sz - j;
}

int findMarker(FILE* f, char* buf, int sz) {
	int a, c, len, l;

	c = a = l = 0;
	while (!feof(f)) {
		buf[c++] = fgetc(f);
		if (c == sz) {
			if (!(a = checkBuf(buf, sz))) {
				break;
			} else {
				l += a;
				c = pushBuf(buf, a, sz);
			}
		}
	}
	return l + sz;
}

int main(int argc, char** argv) {
	char buf[14];
	FILE* f;
	int pct, msg;

	f = fopen("input.txt", "r");
	pct = findMarker(f, buf, 4);
	msg = findMarker(f, buf, 14);

	printf("found after %i %i %i\n", pct, msg, ftell(f));
	return 0;
}
