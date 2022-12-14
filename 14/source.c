
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//#define P2 1

#define W 4096
#define H 200
char data[W][H];

void readIt() {
	int a, b, c, d, i, j, k, h = 0;
	FILE* f;

	memset(data, ' ', W * H);
	f = fopen("input.txt", "r");
	
	while (!feof(f)) {
		fscanf(f, "%i,%i", &a, &b);
		while (fscanf(f, " -> %i,%i", &c, &d) == 2) {
			if (a == c) {
				j = (d < b) ? b - d : d - b;
				k = (d < b) ? d : b;
				for (i = 0; i <= j; ++i) {
					data[a][k + i] = '#';
				}

				h = k + j > h ? k + j : h;
			}
			else {
				j = (c < a) ? a - c : c - a;
				k = (c < a) ? c : a;
				for (i = 0; i <= j; ++i) {
					data[k + i][b] = '#';
				}

				h = b > h ? b : h;
			}
			a = c;
			b = d;
		}
	}

#ifdef P2
	for (i = 0; i < W; ++i) {
		data[i][h + 2] = '#';
	}
#endif //P2

	fclose(f);
}

int simulateP2() {
	int sX, sY, i = 0;
	do {
		sX = 500;
		sY = 0;
		i++;
		while (1) {
			assert(sY < H);
			if (data[sX][sY + 1] == ' ') {
				sY++;
				continue;
			}
			assert(sX >= 0);
			if (data[sX - 1][sY + 1] == ' ') {
				sY++;
				sX--;
				continue;
			}
			assert(sX < W);
			if (data[sX + 1][sY + 1] == ' ') {
				sY++;
				sX++;
				continue;
			}
			break; // at rest
		}
		data[sX][sY] = 'o';
	} while (sY != 0);

	return i;
}

int simulateP1() {
	int sX, sY, i = 0;
	do {
		sX = 500;
		sY = 0;
		i++;
		while (1) {
			if (data[sX][sY + 1] == ' ') {
				sY++;
				continue;
			}
			if (sX >= 0 && data[sX - 1][sY + 1] == ' ') {
				sY++;
				sX--;
				continue;
			}
			if (sX < W && data[sX + 1][sY + 1] == ' ') {
				sY++;
				sX++;
				continue;
			}
			break; // at rest
		}
		data[sX][sY] = 'o';
	} while (sX >= 0 && sY < W);
	return i - 1;
}


void printStuff() {
	int x, y;
	FILE* f;

	f = fopen("output.txt", "w");
	for (y = 0; y < H; ++y) {
		for (x = 0; x < W; ++x) {
			fputc(data[x][y], f);
		}
		fputc('\n', f);
	}
	fclose(f);
}


int main(int argc, char** argv) {
	int i;
	readIt();
#ifdef P2
	i = simulateP2();
#else
	i = simulateP1();
#endif
	printStuff();
	printf("%i \n", i);
}
