#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "input2.txt"
#define ISTR "Blueprint %i: Each ore robot costs %i ore. Each clay robot costs %i ore. Each obsidian robot costs %i ore and %i clay. Each geode robot costs %i ore and %i obsidian."

typedef enum { GEO, OBS, CLAY, ORE } EStuff;

typedef struct {
	int idx;
	int cost[4][4];
} BluePrint;

typedef struct {
	int rob[4], num[4];
} Stuff;

BluePrint bp;
BluePrint* readIt(FILE* f) {
	memset(&bp, sizeof(BluePrint), 0);
	if(fscanf(f, ISTR, &bp.idx, &bp.cost[ORE][ORE], &bp.cost[CLAY][ORE], &bp.cost[OBS][ORE], &bp.cost[OBS][CLAY], &bp.cost[GEO][ORE], &bp.cost[GEO][OBS] ) == 7) {
		return &bp;
	}
	return NULL;
}

int simulate(BluePrint* bp, int* bots, int n) {
	int i, j, t, q[4] = {0,0,0,0};
	Stuff stuff;	

	memset(&stuff, 0, sizeof(Stuff));
	stuff.rob[ORE] = 1;
	t = 0;
	
	while(t < 24) {

		for(j=0; j<4; ++j) {
			for(i=0; i<4; ++i) {
				if(bp->cost[j][i] > stuff.num[i]) break; // can we build it?
			}
			
			if(i==4) {
				q[j]++;
				for(i=0; i<4; ++i) {
					stuff.num[i] -= bp->cost[j][i];
				}
				break;
			}
		}

		for(i=0; i<4; ++i) {
			stuff.num[i] += stuff.rob[i];
			stuff.rob[i] += q[i];
			q[i] = 0;
		}
		
		t++;
	}

	return stuff.num[GEO];
}

int feedSim(BluePrint* bp) {
	
}

int main(int argc, char** argv) {
	int score[16], i, j, n;
	BluePrint* bp;

	n = i = 0;
	FILE* f = fopen(INPUT, "r");
	if((bp = readIt(f))) {
		score[i++] = simulate(bp);
	}

	for(j=0; j<i; ++j)
		n += score[i] * (i + 1);
	printf("got %i\n", n);
	fclose(f);
}
