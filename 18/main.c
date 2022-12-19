#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#define INPUT "input.txt"
#define D 32

char shape[D][D][D];

void readIt() {
	int i;
	int data[3];
	FILE* f = fopen(INPUT, "r");

	memset(shape, 0, D * D * D);
	while(fscanf(f, "%i,%i,%i\n", data, data + 1, data + 2) == 3) {
		assert(data[0] < D);
		assert(data[1] < D);
		assert(data[2] < D);
		
		printf("%i,%i,%i\n", data[0], data[1], data[2]);
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
	int x,y,k;
	int s,t,n = 0;

	for(y=0; y<D; ++y) {
		for(x=0; x<D; ++x) {
			t = s = 2; // assume steam
			for(k=0; k<D; ++k) {
				t = shape[x][y][k];
				n += (t != s && t && s) ? 1 : 0;
				s = t;
			}
		}
	}

	for(y=0; y<D; ++y) {
		for(k=0; k<D; ++k) {
			t = s = 2; // assume steam
			for(x=0; x<D; ++x) {
				t = shape[x][y][k];
				n += (t != s && t && s) ? 1 : 0;
				s = t;
			}
		}
	}

	for(k=0; k<D; ++k) {
		for(x=0; x<D; ++x) {
			t = s = 2; // assume steam
			for(y=0; y<D; ++y) {
				t = shape[x][y][k];
				n += (t != s && t && s) ? 1 : 0;
				s = t;
			}
		}
	}

	return n;
}

int main(int argc, char** argv) {
	int i;
	readIt();
	floodIt(0,0,0);
	i = rayCast();
	printf("got %i\n", i);
}
