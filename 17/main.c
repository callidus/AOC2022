#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#define INPUT "input.txt"
#define SPACE 12000

#define COL 7
#define ROW 10000

char data[ROW][COL];

typedef struct {
	const char d[4][4];
	int w, h;
} Rock;

Rock rocks[] = { // tetris :D
	{.d = {"@@@@", 
		   "....",
		   "....",
		   "...."},
		.w = 4,
		.h = 1,
	},

	{.d = {".@..", 
		   "@@@.",
		   ".@..",
		   "...."},
		.w = 3,
		.h = 3,
	},

	{.d = {"@@@.", 
		   "..@.",
		   "..@.",
		   "...."},
		.w = 3,
		.h = 3,
	},

	{.d = {"@...", 
		   "@...",
		   "@...",
		   "@..."},
		.w = 1,
		.h = 4,
	},

	{.d = {"@@..", 
		   "@@..",
		   "....",
		   "...."},
		.w = 2,
		.h = 2,
	},
};


char* jets = NULL;
int numJets = 0;
void readIt() {
	int i;
	FILE* f = fopen(INPUT, "r");
	jets = calloc(1, SPACE);
	while((jets[numJets++] = fgetc(f)) != '\n'); // simplest read yet 
	assert(numJets < SPACE);
	numJets--;
	fclose(f);
}

int testRock(int rx, int ry, Rock* r) {
	int col, row;
	if(rx + r->w > COL) return 0;
	if(rx < 0) return 0;
	if(ry < 0) return 0;

	for(row=0; row<r->h; ++row) {
		for(col=0; col<r->w; ++col) {
			if(r->d[row][col] == '@' && data[ry+row][rx+col] == '#') return 0;
		}
	}
	
	return 1;
}

int commitRock(int rx, int ry, Rock* r) {
	int col,row;
	for(row=0; row<r->h; ++row) {
		for(col=0; col<r->w; ++col) {
			data[ry+row][rx+col] = (r->d[row][col] == '@') ? '#' : data[ry+row][rx+col];
		}
	}
	return ry + r->h;
}

void draw(Rock* r, int rx, int ry, int s, char c) {
	int row,col;
	for(row=s; row>=0; --row) {
		putchar('|');
		for(col=0; col<COL; ++col) {
			if(row >= ry && row < ry + r->h && col >= rx && col < rx+r->w) 
				putchar(r->d[row - ry][col - rx]);
			else
				putchar(data[row][col]);
		}
		puts("|");
	}
	printf("+-------+  [%c] (%i %i)\n", c, rx, ry);
	puts("");
}

long long int scroll(long long int highMark, int n) {
	highMark -= n;
	memmove(data[0], data[n], highMark * COL);
	memset(data[highMark], ' ', COL * n);
	return highMark;
}

// part 2 stuff here
typedef struct  {
	int j,r,height,count;
} CycleMarker;
CycleMarker cycle[2];

int cIdx = 0;
long long int cDiv = 0;
long long int cMod = 0;
//long long int targ = 2022; 
long long int targ = 1000000000000ll;
                      
int noteCycle(int j, int r, int height, int count) {
	long long int ta, tb;
	printf("cycle at j %i r %i height %i rocks %i\n", j, r, height, count);
	cycle[cIdx++] = (CycleMarker){ .j = j, .r = r, .count = count, .height = height };

	if(cIdx == 2) {
		ta = cycle[1].height - cycle[0].height;
		tb = cycle[1].count - cycle[0].count;

		printf("h dif: %i c dif: %i\n", ta, tb);

		cDiv = targ / tb;
		cMod = targ % tb;

		printf("so div %lli, mod %lli\n", cDiv, cMod);
		return 0; //cycle[0].count - cMod;
	}
	return 1;
}

void closeCycle(int height, int count) {
	int a,b,c;
	long long int val = 0;

	val = cycle[0].height;
	val += cDiv * (cycle[1].height - cycle[0].height);

	a = (height - cycle[1].height);
	val += a;

	printf("got %lli (%lli)\n", val);
	printf(""); 
}
// ----------

long long int simulate() {
	int rx, ry, rIdx, jIdx, pjIdx, loop, cycle;
	long long int highMark, tempMark, t, count;
	Rock* rock;

	pjIdx = count = rIdx = jIdx = highMark = tempMark = 0;
	loop = 1600;

	cycle = 0;

	memset(data, ' ', COL*ROW);
	while(loop--) {
		/* // once I found the divisor (the cycle) I just worked it out 
		if(!cDiv && rIdx == 0 && jIdx < pjIdx && (cycle == 0 || cycle == jIdx)) { // find cycle
			//printf("derp %i %i %i (%i)\n", rIdx, jIdx, count, tempMark + highMark);
			loop = noteCycle(jIdx, rIdx, tempMark + highMark, count);
			if(!loop) break;
			cycle = jIdx;
		}
		pjIdx = jIdx;
		//if(cDiv) {
		//	loop--; // start close cycle
		//}
		*/
		

		// spawn rock
		count++;
		rock = rocks + rIdx;
		rIdx = (rIdx + 1) % 5;
		rx = 2;
		ry = highMark + 3;

		//draw(rock, rx, ry, highMark + 10, ' ');
		while(1) {	
			// jets
			rx += (jets[jIdx] == '>') ? 1 : -1;
			if(!testRock(rx, ry, rock)) {
				rx += (jets[jIdx] == '>') ? -1 : 1;
			}
			//draw(rock, rx, ry, highMark + 10, jets[jIdx]);
			jIdx = (jIdx + 1) % numJets;

			// fall
			ry--; 
			if(!testRock(rx, ry, rock)) {
				ry++;
				t = commitRock(rx, ry, rock);
				highMark = (highMark > t ) ? highMark : t;
				if(highMark > 1000) {
					highMark = scroll(highMark, 50);
					tempMark += 50;
				}
				break;
			}
			//draw(rock, rx, ry, highMark + 10, 'v');
		}
	}
	closeCycle(tempMark + highMark, count);
	//draw(rock, 2, highMark + 3, highMark + 10, ' ');
	return highMark + tempMark;
}

int main(int argc, char** argv) {
	long long int i, j;
	readIt();


	// hacks - 
	// - I run the sim to find the divisor and the mod
	// - then stop it and hard code the cycle count and mod steps
	// - just add results and the cycles 
	//  - I should have re-started the sim in code after knowing the mod, but whatever

	i = simulate();
	j = 579710144ll * 2694ll + i;
	printf("got %lli (%lli)\n", i, j);
}
