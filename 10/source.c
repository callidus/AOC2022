
#include <stdio.h>
#include <stdlib.h>

int accm = 0;
void trace(int c, int x) {
	int p = (c - 1) % 40;
	(x - 1 == p || x == p || x + 1 == p) ? printf("#") : printf(".");
	if (p == 39) printf("\n");

	if ((c + 20) % 40 == 0) {
		accm += x * c;
	}
}

int main(int argc, char** argv) {
	int x, val, cycle;
	char op[8];
	FILE* f;

	x = 1;
	accm = 0;
	cycle = 0;
	f = fopen("input.txt", "r");
	while (!feof(f)) {
		cycle++;
		trace(cycle, x);
		if (fscanf(f, "%s %i", op, &val) == 2) {
			cycle++;
			trace(cycle, x);
			x += val;
		}
		fgetc(f); // trim
	}
	printf("\n\n");

	printf("cycle %i val %i accm %i\n", cycle, x, accm);
}
