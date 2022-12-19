#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "input.txt"
#define D 32

char shape[D][D][D];

void readIt() {
	int i;
	int data[3];
	FILE* f = fopen(INPUT, "r");

	memset(shape, 0, D * D * D);
	while(fscanf(f, "%i,%i,%i\n", data, data + 1, data + 2) == 3) {
		shape[data[0]][data[1]][data[2]] = 1;
	}
	fclose(f);
}

void floodIt(int x, int y, int z) {
	if(x<0 || x == D ||
	   y<0 || y == D ||
	   z<0 || z == D ||
	   shape[x][y][z] != 0)
		return;

	shape[x][y][z] = 2;
	floodIt(x+1, y, z);
	floodIt(x-1, y, z);
	floodIt(x, y+1, z);
	floodIt(x, y-1, z);
	floodIt(x, y, z+1);
	floodIt(x, y, z-1);
	return;
}

int rayCast() {
	char s[3],t[3];
	int x,y,k, i, n = 0;

	for(y=0; y<D; ++y) {
		for(x=0; x<D; ++x) {
			memset(s, 2, 3); // start in steam
			memset(t, 2, 3);
			for(k=0; k<D; ++k) {
				t[0] = shape[x][y][k];
				t[1] = shape[y][k][x];
				t[2] = shape[k][x][y];
				for(i=0;i<3;++i) {
					n += (t[i] != s[i] && t[i] && s[i]) ? 1 : 0;
					s[i] = t[i];
				}
			}
		}
	}
	return n;
}

int main(int argc, char** argv) {
	readIt();
	floodIt(0,0,0);
	printf("got %i\n", rayCast());
}
