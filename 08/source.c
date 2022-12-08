
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define D

#ifdef D
#define W 99
#define H 99
#define IPT "input.txt"

#else

#define W 5
#define H 5
#define IPT "input2.txt"

#endif

char data[W * H];

void computeCover() {
	char buf[W * H];
	int y, x, k, val, num, vis;

	memset(buf, '-', W * H);
	num = W * 2 + H * 2 - 4; // edges
	for (y = 1; y < H - 1; ++y) {
		for (x = 1; x < W - 1; ++x) {
			val = data[y * W + x];
			vis = 0;

			if (!vis) {
				vis = 1;
				for (k = 0; k < x; ++k) { // left
					if (data[y * W + k] >= val) {
						vis = 0;
						break;
					}
				}
				if (vis) buf[y * W + x] = 'L';
			}


			if (!vis) {
				vis = 1;
				for (k = x + 1; k < W; ++k) { // right
					if (data[y * W + k] >= val) {
						vis = 0;
						break;
					}
				}
				if (vis) buf[y * W + x] = 'R';
			}


			if (!vis) {
				vis = 1;
				for (k = 0; k < y; ++k) { // above
					if (data[k * W + x] >= val) {
						vis = 0;
						break;
					}
				}
				if (vis) buf[y * W + x] = 'A';
			}

			if (!vis) {
				vis = 1;
				for (k = y + 1; k < H; ++k) { // below
					if (data[k * W + x] >= val) {
						vis = 0;
						break;
					}
				}
				if (vis) buf[y * W + x] = 'B';
			}


			num += vis;
		}
	}


	for (y = 0; y < H; ++y) {
		for (x = 0; x < W; ++x) {
			printf("%c", buf[y * W + x]);
		}
		printf("\n");
	}

	printf("\ngot %i vis\n", num);
}

void computeScene() {
	int buf[W * H];
	int x, y, k, visL, visR, visA, visB, val, max;
	for (y = 0; y < H; ++y) {
		for (x = 0; x < W; ++x) {
			val = data[y * W + x];
			
			visL = 0;
			for (k = x-1; k>=0; --k) { // left
				visL++;
				if (data[y * W + k] >= val) {
					break;
				}
			}

			visR = 0;
			for (k = x + 1; k<W; ++k) { // right
				visR++;
				if (data[y * W + k] >= val) {
					break;
				}
			}

			visA = 0;
			for (k = y-1; k >= 0; --k) { // above
				visA++;
				if (data[k * W + x] >= val) {
					break;
				}
			}

			visB = 0;
			for (k = y + 1; k < H; ++k) { // below
				visB++;
				if (data[k * W + x] >= val) {
					break;
				}
			}

			buf[y * W + x] = visA * visL * visR * visB;
		}
	}

	max = 0;
	for (y = 0; y < H; ++y) {
		for (x = 0; x < W; ++x) {
			val = buf[y * W + x];
			max = max > val ? max : val;
		}
	}

	printf("best %i\n", max);
}

int main(int argc, char** argv) {
	int y, x;
	FILE* f;

	f = fopen(IPT, "r");
	for (y = 0; y < H; ++y) {
		for (x = 0; x < W; ++x) {
			data[y * W + x] = fgetc(f) - 48;
		}
		fgetc(f); // strip
	}

	for (y = 0; y < H; ++y) {
		for (x = 0; x < W; ++x) {
			printf("%i", data[y * W + x]);
		}
		printf("\n");
	}
	printf("\n");

	computeCover();
	computeScene();
}
