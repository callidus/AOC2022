
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct {
	long long int atX, atY, byX, byY, d;
} Sensor;

int numS = 0;
Sensor sens[32];

#define D

#ifdef D
#define CHUNK 1000
#define SPACE 4000000
#define INPUT "input.txt"
#else
#define CHUNK 2
#define SPACE 20
#define INPUT "input2.txt"
#endif


void readIt() {
	FILE* f = fopen(INPUT, "r");	
	while (!feof(f)) {
		fscanf(f, "Sensor at x=%lli, y=%lli: closest beacon is at x=%lli, y=%lli\n", 
				&sens[numS].atX, &sens[numS].atY,
				&sens[numS].byX, &sens[numS].byY);
		sens[numS].d = llabs(sens[numS].atX - sens[numS].byX) +
			           llabs(sens[numS].atY - sens[numS].byY);
		numS++;
	}
	fclose(f);
}


void scanChunk(long long int startX, long long int startY) {
	long long int x, y, i, m, n;
	Sensor temp[32];
	long long int d[4];

	n = 0;
	for (i = 0; i < numS; ++i) { // cull stuff
		d[0] = llabs(sens[i].atX - startX)           + llabs(sens[i].atY - startY);
		d[1] = llabs(sens[i].atX - (startX + CHUNK)) + llabs(sens[i].atY - startY);
		d[2] = llabs(sens[i].atX - startX)           + llabs(sens[i].atY - (startY + CHUNK));
		d[3] = llabs(sens[i].atX - (startX + CHUNK)) + llabs(sens[i].atY - (startY + CHUNK));

		if (d[0] <= sens[i].d && 
			d[1] <= sens[i].d && 
			d[2] <= sens[i].d && 
			d[3] <= sens[i].d)
			return; // cull entire chunk
		
		if (d[0] > sens[i].d && 
			d[1] > sens[i].d && 
			d[2] > sens[i].d && 
			d[3] > sens[i].d) {
			// cull entire sensor
		} else {
			temp[n++] = sens[i];
		}
	}

	if (n == 0) return;
	
	for (x = startX; x < startX + CHUNK; ++x) {
		for (y = startY; y < startY + CHUNK; ++y) {
			m = 1;
			for (i = 0; i < n; ++i) {
				if (x < temp[i].atX - temp[i].d || x > temp[i].atX + temp[i].d ||
					y < temp[i].atY - temp[i].d || y > temp[i].atY + temp[i].d)
					continue; // maybe faster, not sure

				if (llabs(temp[i].atX - x) + llabs(temp[i].atY - y) <= temp[i].d) {
					m = 0;
					break;
				}
			}

			if (m) {
				printf("x %lli y %lli\n", x, y);
			}
		}
	}
}


int main(int argc, char** argv) {
	long long int x, y, t, s;
	readIt();

	
	/* part 1
	long long int minx, miny, maxx, maxy;
	minx = miny = maxx = maxy = 0;
	for (i = 0; i < numS; ++i) {
		minx = min(minx, sens[i].atX - sens[i].d);
		miny = min(miny, sens[i].atY - sens[i].d);
		maxx = max(maxx, sens[i].atX + sens[i].d);
		maxy = max(maxy, sens[i].atY + sens[i].d);
	}
	
	n = 0;
	y = 2000000;
	for (x = minx; x <maxx; ++x) {
		for (i = 0; i < numS; ++i) {
			d = llabs(sens[i].atX - x) + llabs(sens[i].atY - y);
			if (d <= sens[i].d) {
				n++;
				break;
			}
		}
	}
	printf("%llu \n", n - 1);
	*/

	s = (SPACE / CHUNK); 
	assert(SPACE % CHUNK == 0);
	t = s * s;
	for (y = 0; y < s; ++y) {
		for (x = 0; x < s; ++x) {
			//printf("%lli %lli (%lli)\n", x, y, t);
			scanChunk(x * CHUNK, y * CHUNK);
			t--;
		}
	}
}

/*
x 2895970 y 2601918

11583882601918
*/