#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <math.h>

#define D

#ifdef D

#define INPUT "input.txt"
#define C 150
#define R 200
#define Q 50

#else

#define INPUT "input2.txt"
#define C 16
#define R 12
#define Q 4

#endif //D

char lut[4][5] = { // directions: row inc, col inc, mark, 90 deg l, 90 deg r
	{ 0, 1, '>', 1, 3}, 
	{ 1, 0, 'v', 3, 0},
	{ 0,-1, '<', 4, 1},
	{-1, 0, '^', 0, 2},
};

typedef struct{
	int num;
	int type;
} Move;

char data[R][C];

typedef struct {
	int adj[4]; // adjacent
	int dir[4]; // new dir
	int minR, minC;
} Face;

#ifdef D

Face face[6] = { // just some data, matched to directions
	(Face){.adj={1,2,3,5}, .dir={0,1,0,0}, .minR=0,   .minC=50 },  // 0
	(Face){.adj={4,2,0,5}, .dir={2,2,2,3}, .minR=0,   .minC=100},  // 1
	(Face){.adj={1,4,3,0}, .dir={3,1,1,3}, .minR=50,  .minC=50 },  // 2
	(Face){.adj={4,5,0,2}, .dir={0,1,0,0}, .minR=100, .minC=0  },  // 3
	(Face){.adj={1,5,3,2}, .dir={2,2,2,3}, .minR=100, .minC=50 },  // 4
	(Face){.adj={4,1,0,3}, .dir={3,1,1,3}, .minR=150, .minC=0  },  // 5
};

#else

Face face[6] = { // just some data, matched to directions (test data)
	(Face){.adj={5,3,2,1}, .dir={2,1,1,1}, .minR=0, .minC=8  },  // 0
	(Face){.adj={2,4,5,0}, .dir={0,3,3,2}, .minR=4, .minC=0  },  // 1
	(Face){.adj={3,4,1,0}, .dir={0,0,2,0}, .minR=4, .minC=4  },  // 2
	(Face){.adj={5,4,2,0}, .dir={1,1,2,3}, .minR=4, .minC=8  },  // 3
	(Face){.adj={5,1,2,3}, .dir={0,3,3,3}, .minR=8, .minC=8  },  // 4
	(Face){.adj={0,1,4,3}, .dir={2,0,2,2}, .minR=8, .minC=12 },  // 5
};

#endif // D

Move* moves;
int num = 0;
int max = 0;

void pushMove(Move* n) {
	moves = (num == max) ? realloc(moves, (max += 10) * sizeof(Move)) : moves;
	moves[num++] = *n;
}

void printMap() {
	int r,c;
	system("clear");
	for(r=0;r<R; ++r) {
		for(c=0;c<C; ++c) {
			printf("%c", data[r][c]);
		}
		printf("\n");
	}
	printf("\n");
}

void printMoves() {
	int i;
	for(i=0; i<num; ++i) {
		if(moves[i].type == 'M')
			printf("%i\n", moves[i].num);
		else
			printf("%c\n", moves[i].type);
	}
}

void convertPoint(int *r, int *c, int* d) {
	int tr, tc;
	char ta, tb;
	
	int t, f, fr, fc, fd;
	for(f=0; f<6; ++f) { // what face am I in ?
		if(*r >= face[f].minR && 
			 *r < face[f].minR + Q &&
			 *c >= face[f].minC &&
			 *c < face[f].minC + Q){
			break;
		}
	}
	
	// convert to face space
	fr = (*r - face[f].minR) - Q / 2;
	fc = (*c - face[f].minC) - Q / 2;
	fd = face[f].dir[*d];
	
	if(lut[*d][3] == fd) { // l 90
		t = fr;
		fr = -fc;
		fc = t;
	} else if(lut[*d][4] == fd) { // r 90
		t = fr;
		fr = fc;
		fc = -t;
	} else if(*d != fd) { // 180
		fr = -fr;
		fc = -fc;
	}

	tr = *r;
	tc = *c;
	
	// convert back to map space
	f = face[f].adj[*d];
	*r = face[f].minR + (fr + Q / 2);
	*c = face[f].minC + (fc + Q / 2);
	*d = fd;
	
	switch(fd){ // translate
		case 0: fc = face[f].minC; break;
		case 1: fr = face[f].minR; break;
		case 2: fc = face[f].minC + Q - 1; break;
		case 3: fr = face[f].minC + Q - 1; break;
	}
	
	
	// /*
	ta = data[tr][tc];
	tb = data[*r][*c];
	
	data[tr][tc] = 'X';
	data[*r][*c] = 'Y';
	
	printMap();
	
 	data[tr][tc] = ta;
	data[*r][*c] = tb;
	// */
}

void readIt() {
	int r, c, t;
	char d;
	Move m;
	FILE* f = fopen(INPUT, "r");
	
	memset(data, ' ', R * C);
	
	r = 0;
	do {
		c = 0;
		while((d = fgetc(f)) !='\n') {
			data[r][c] = d;
			c++;
		}
		r++;
	} while(c != 0);

	while(fscanf(f, "%i%c", &t, &d) == 2) {
		m.type = 'M';
		m.num = t;
		pushMove(&m);
		
		m.type = d;
		m.num = 0;
		pushMove(&m);
	}
	num--;
	fclose(f);
}


void move(int *r, int *c, int *d) {
	int tr, tc;
	tr = *r + lut[*d][0];
	tc = *c + lut[*d][1];
	
	if(tc==C || tc < 0 || tr==R || tr < 0 || data[tr][tc] == ' ') { // off edge of map, or tile
		convertPoint(r, c, d);
	} else {
		*r = tr;
		*c = tc;
	}
}

void walkIt() {
	int tr, tc, td, r, c, d, m, i, j;
	
	i = td = tr = tc = 0;
	while(data[0][tc++] != '.'); // find start
	tc--;
	
	r = tr;
	c = tc;
	d = td;
	while(i<num) {
		m = moves[i++].num;
		for(j=0; j<m; ++j) {
			data[r][c] = lut[d][2];
			move(&tr, &tc, &td);
			
			if(data[tr][tc]=='#') { // wall
				tr = r;
				tc = c;
				td = d;
				break;
			}
			
			r = tr;
			c = tc;
			d = td;
		}
		
		d = d + ((moves[i++].type == 'R') ? 1 : -1);
		d = (d == 4) ? 0 : d;
		d = (d == -1) ? 3 : d;
		td = d;
	}
	
	c++;
	r++;
	d = (d + 1)  % 4; // rouge '\n' in input
	printf("at %i %i %i [%i]\n", c, r, d, 1000 * r + 4 * c + d);
}

int main(int argc, char** argv) {
	readIt();
	//printMap();
	//printMoves();
	walkIt();
	printMap();
}
