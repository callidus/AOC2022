
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#define INPUT "input.txt"
#define SPACE 128
#define INF 9
#define MINS 26

struct Valve {
	char name[3];
	int flow;
};

typedef struct Valve Valve;


int numV = 0;
Valve valve[SPACE];

// for Floyd Warshall
int dp[SPACE][SPACE];
int n[SPACE][SPACE];
int m[SPACE][SPACE];

// for path search
int path[SPACE];
int visit[SPACE];
int pathLen;

void addRoute(int v, const char* to) {
	int i;
	for (i = 0; i < numV; ++i) {
		if (strcmp(valve[i].name, to) == 0) {
			m[v][i] = 1;
			break;
		}
	}
}

void readItA() {
	FILE* f = fopen(INPUT, "r");
	while (!feof(f)) { // read in names and flow
		memset(valve + numV, 0, sizeof(Valve));
		if(fscanf(f, "Valve %s has flow rate=%i; tunnels lead to valve", valve[numV].name, &valve[numV].flow) != 2)
            return;
		while (fgetc(f) != '\n' && !feof(f));
        numV++;
	}
	fclose(f);
}

void readItB() {
	int k, a;
	char tmp[3];
	FILE* f = fopen(INPUT, "r");

	k = 0;
	memset(tmp, 0, 3);
	while (!feof(f)) { // read in names and flow
		if(fscanf(f, "Valve %s has flow rate=%i; tunnels lead to valve", valve[k].name, &valve[k].flow) != 2)
            return;
		while (fgetc(f) != ' ');

		printf("node %s\n", valve[k].name);

		do { // read in and link routes
			fscanf(f, "%c%c,", tmp, tmp + 1);
			addRoute(k, tmp);
		} while (fgetc(f) != '\n' && !feof(f));
		k++;
	}
	fclose(f);
}

void FloydWarshall() {
	int i, j, k, idx;

	// setup dp, n
	for(i=0; i<numV; ++i) {
		for(j=0; j<numV; ++j) {
			dp[i][j] = m[i][j];
			n[i][j] = (m[i][j] != INF) ? j : INF;
		}
	}

	// solve
	for(k=0; k < numV; ++k) {
		for(i=0;  i< numV; ++i) {
			for(j=0; j < numV; ++j) {
				if(dp[i][k] + dp[k][j] < dp[i][j]) {
					dp[i][j] = dp[i][k] + dp[k][j];
					n[i][j] = n[i][k];
				}
			}
		}
	}

	for(i=0; i<numV; ++i) {
		for(j=0; j<numV; ++j) {
			printf("%i ", dp[i][j]);
		}
		printf("\n");
	}
}

int best = 0;
int tests = 0;

typedef struct {
	int bits, score;
} Val;

Val* saved = NULL;
int numSaved;
int maxSaved;

int scorePath(int* a, int l) {
	int i, t, s, at, x = 0;

	tests++;
	s = t = 0;
	at = a[0];
	for(i=1; i < l; ++i) {
		t += (dp[at][a[i]] + 1);
		s += valve[a[i]].flow * (MINS - t);

		//printf("%s -> %s (%i),", valve[at].name , valve[a[i]].name, t);
		at = a[i];

		x |= (1 << at);
	}

	//printf(" [%i] \n", t);

	best = (s > best) ? s : best;
	saved = (numSaved == maxSaved) ? realloc(saved, (maxSaved += 1024) * sizeof(Val)) : saved;
	saved[numSaved++] = (Val) {.bits = x, .score = s};

	return s;
}

int visits = 0;
void dfs(int* p, int pLen, int t) {

	int i, a;
	if(visits == pathLen) {
		scorePath(p - 1, pLen + 1);
		return;
	}

	if(t >= MINS) {
		/* nope :-(
		if(!base) {
			base = scorePath(p - 1, pLen, base);
			dfs(p, 0, 0, base); // jumbo's turn
		} else {
			scorePath(p - 1, pLen, base);
		}
		//*/
		scorePath(p - 1, pLen);
		return;
	}

	for(i=0; i<pathLen; ++i) {
		if(!visit[i]) {
			p[pLen] = path[i];
			a = dp[p[pLen-1]][path[i]];
			visit[i] = 1;
			visits++;
			dfs(p, pLen + 1, t + a + 1);
			visits--;
			visit[i] = 0;
		}
	}
}


int main(int argc, char** argv) {
	int i, j, xa, xb, p[SPACE];
	Valve* t;

	// setup m
	for(i=0; i<SPACE; ++i) {
		for(j=0; j<SPACE; ++j) {
			m[i][j] = (i == j) ? 0 : INF;
		}
	}

	readItA();
	readItB();

	pathLen = 0;  // fill in path array
	memset(visit, 0, SPACE * sizeof(int));
	for(i=0;i<numV; ++i) {
		if(valve[i].flow > 0) {
			path[pathLen++] = i;
		}
		if(strcmp(valve[i].name, "AA") == 0) {
			p[0] = i; // start at AA, dont permute
		}
	}

	FloydWarshall();
	printf("\n");

	dfs(p + 1, 0, 0);


	printf("%i , %i\n", tests, best);

	xa = 0;
	for(i=0; i<numSaved - 1; ++i) {
		for(j=i+1; j<numSaved; ++j) {
			if(!(saved[i].bits & saved[j].bits)) {
				xa = (saved[i].score + saved[j].score > xa) ? saved[i].score + saved[j].score : xa;
			}
		}
	}
	printf("maybe? %i\n", xa);
}
