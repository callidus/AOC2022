
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define W 512
#define H 512
char buf[W][H];

#define L 10
int rope[L][2];

void vis(int hx, int hy, int tx, int ty) {
	int x, y;
	FILE* f;

	f = fopen("output2.txt", "w");
	fprintf(f, "\n");
	for (y = 0; y < H; ++y) {
		for (x = 0; x < W; ++x) {
			if (hx == x && hy == y) fprintf(f, "H");
			else if (tx == x && ty == y) fprintf(f, "T");
			else if (buf[x][y]) fprintf(f, "o");
			else fprintf(f, ".");
		}
		fprintf(f, "\n");
	}
	fclose(f);
}

int main(int argc, char** argv) {
	int hx, hy, tx, ty, num, i, j;
	char dir;
	FILE* f;

	f = fopen("input.txt", "r");
	memset(buf, 0, W * H);
	for (i = 0; i < L; ++i) {
		rope[i][0] = W / 2;
		rope[i][1] = H / 2;
	}
	
	//vis(hx, hy, tx, ty);

	while (!feof(f)) {
		fscanf(f, "%c %i\n", &dir, &num);
		printf("\n== %c %i ==\n", dir, num);
		for (i = 0; i < num; ++i) {
			switch (dir) {
			case 'R':
				rope[0][0]++;
				
				break;

			case 'L':
				rope[0][0]--;
				break;

			case 'U':
				rope[0][1]--;
				break;

			case 'D':
				rope[0][1]++;
				break;
			}

			buf[rope[9][0]][rope[9][1]] = 1;

			for (j = 0; j < L-1; ++j) {
				
				if (abs(rope[j][0] - rope[j + 1][0]) > 1) {
					rope[j + 1][0] += rope[j][0] < rope[j+1][0] ? -1 : 1;

					rope[j + 1][1] += rope[j][1] < rope[j+1][1] ? -1 : 0;
					rope[j + 1][1] += rope[j][1] > rope[j+1][1] ? 1 : 0;
				}

				if (abs(rope[j][1] - rope[j + 1][1]) > 1) {
					rope[j + 1][1] += rope[j][1] < rope[j + 1][1] ? -1 : 1;

					rope[j + 1][0] += rope[j][0] < rope[j + 1][0] ? -1 : 0;
					rope[j + 1][0] += rope[j][0] > rope[j + 1][0] ? 1 : 0;
				}
			}
			
			//vis(hx,hy,tx,ty);
		}
	}
	buf[rope[9][0]][rope[9][1]] = 1;

	num = 0;
	for (i = 0; i < H; ++i) {
		for (j = 0; j < W; ++j) {
			num += buf[i][j];
		}
	}

	//vis(hx, hy, tx, ty);
	printf("got %i\n", num);
}
