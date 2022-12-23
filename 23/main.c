#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <math.h>

// the size of a bucket must never be less then 5x5
#define B 10

int move[4][2] = {
	{-1, 0},
	{ 1, 0},
	{ 0,-1},
	{ 0, 1},
};

typedef struct {
	int r,c, pr, pc;
} Elf;

typedef struct {
	int minR, maxR, minC, maxC;
	int num, max, idx;
	Elf** data;
} Bucket;

Bucket bucket[B][B];
Elf* elf;
int turn = 0;
int dirty = 0;
int num = 0;
int max = 0;
int minC, minR, maxC, maxR;

void pushElf(Elf* e) {
	elf = (num == max) ? realloc(elf, (max += 10) * sizeof(Elf)) : elf;
	elf[num++] = *e;
}

void readIt() {
	Elf e;
	int t, c;
	
	FILE* f = fopen("input.txt", "r");
	minR = minC = maxC = maxR = c = 0;
	e.pr = e.pc = 1000;
	while(!feof(f)) {
		c = 0;
		while((t = fgetc(f)) != '\n' && !feof(f)) {
			if(t=='#'){
				e.r = maxR;
				e.c = c;
				pushElf(&e);
			}
			c++;
			maxC = c;
		}
		
		maxR++;
	}
	fclose(f);
}

int gatherElves(Elf* at, Elf** near, int n, Bucket* b, int rad) {
	int i, d;
	
	for(i=0; i<b->num; ++i) {
		if((abs(at->r - b->data[i]->r) <= rad && abs(at->c - b->data[i]->c) <= rad)){
			if(b->data[i] != at)
				near[n++] = b->data[i];
		}
	}
	return n;
}

int gatherBuckets(Elf* at, int rad, Bucket** out) {
	int i, n, br, bc, p[4][2];
	Bucket* b;
	
	p[0][0] = at->r - rad;
	p[0][1] = at->c - rad;
	
	p[1][0] = at->r - rad;
	p[1][1] = at->c + rad;
	
	p[2][0] = at->r + rad;
	p[2][1] = at->c + rad;
	
	p[3][0] = at->r + rad;
	p[3][1] = at->c - rad;
	
	n = 0;
	for(br=0;br<B; ++br) {
		for(bc=0;bc<B; ++bc) {
			b = &bucket[br][bc];
			
			for(i=0;i<4;++i) {
				if(p[i][0] >= b->minR && p[i][0] < b->maxR && 
					 p[i][1] >= b->minC && p[i][1] < b->maxC ) {
					assert(n<4);
					out[n++] = b;
					break;
				}
			}
			
		}
	}
	
	return n;
}

void considerMove(Elf* at, Elf** near, int num) {
	int i, j, n, m[4] = {1,1,1,1};
	
	for(i=0; i<num; ++i) {
		if(near[i]->r < at->r) m[0] = 0;
		if(near[i]->r > at->r) m[1] = 0;
		if(near[i]->c < at->c) m[2] = 0;
		if(near[i]->c > at->c) m[3] = 0;
	}
	
	n = 0;
	for(i=0; i<4; ++i) {
		if(m[i]) n++;
	}
	
	if(n == 4 || n == 0) {
		at->pr = at->r;
		at->pc = at->c;
		return;
	}
	
	for(i=0; i<4; ++i) {
		j = (i + turn) % 4;
		if(m[j]) {
			at->pr = at->r + move[j][0];
			at->pc = at->c + move[j][1];
			return;
		}
	}
	assert(0);
}

void performMove(Elf* at, Elf** near, int num) {
	int i;
	for(i=0; i<num; ++i) {
		if(near[i]->pr == at->pr && near[i]->pc == at->pc) {
			break;
		}
	}
	
	if(i == num) { // make the move
		if(at->r != at->pr || at->c != at->pc) {
			dirty = 1;
			at->r = at->pr;
			at->c = at->pc;
			
			minR = (at->r < minR) ? at->r : minR;
			maxR = (at->r > maxR) ? at->r : maxR;
			minC = (at->c < minC) ? at->c : minC;
			maxC = (at->c > maxC) ? at->c : maxC;
		}
	}
}

void addToBucket(Bucket* b, Elf* e) {
	b->data = (b->num == b->max) ? realloc(b->data, (b->max += 10) * sizeof(Elf*)) : b->data;
	b->data[b->num++] = e;
}

void buildBuckets() {
	int br, bc, w, h, i, idx = 0;
	h = (maxR - minR) / B;
	w = (maxC - minC) / B;
	
	// make spacial buckets covering full area
	for(br=0;br<B; ++br) {
		for(bc=0;bc<B; ++bc) {
			bucket[br][bc].idx = idx++;
			bucket[br][bc].num = 0;
			
			bucket[br][bc].minR = br * h + minR;
			bucket[br][bc].minC = bc * w + minC;
			
			bucket[br][bc].maxR = (br + 1) * h + minR;
			bucket[br][bc].maxC = (bc + 1) * w + minC;
		}
	}
	
	for(br=0;br<B; ++br) { // leftovers
		bucket[br][B-1].maxC += (maxC - minC) % B; 
	}
	
	for(bc=0;bc<B; ++bc) { // leftovers
		bucket[B-1][bc].maxR += (maxR - minR) % B; 
	}
	
	// put elves in buckets
	for(i=0; i<num; ++i) {
		br = (elf[i].r - minR) / h;
		bc = (elf[i].c - minC) / w;
		br = (br > B-1) ? B-1 : br; // leftovers
		bc = (bc > B-1) ? B-1 : bc; // leftovers
		addToBucket(&bucket[br][bc], elf + i);
	}
}

Bucket* bucketFromPoint(int r, int c) {
	int br, bc;
	Bucket* b;
	for(br=0;br<B;++br) {
		for(bc=0;bc<B;++bc) {
			b = &bucket[br][bc];
			if(r >= b->minR &&  r < b->maxR && 
					c >= b->minC && c < b->maxC ) {
				return b;
			}
		}
	}
	
	assert(0);
	return NULL;
}

void printElves() {
	int i;
	for(i=0; i<num; ++i) {
		printf("{%i, %i} -> {%i, %i}\n", elf[i].r, elf[i].c, elf[i].pr, elf[i].pc );
	}
	printf("\n");
}

void printBuckets() {
	int r,c,i;
	char out;
	Bucket* b;
	
	for(r=minR;r<maxR;++r) {
		for(c=minC;c<maxC;++c) {
			b = bucketFromPoint(r, c);
			out = 97 + b->idx;
			
			for(i=0;i<b->num; ++i) {
				if(b->data[i]->r == r && b->data[i]->c == c) {
					out -= 32;
					break;
				}
			}
			printf("%c", out);
		}
		printf("\n");
	}
}

void printMap() {
	int r,c,i;
	char out;
	
	for(r=minR;r<maxR;++r) {
		for(c=minC;c<maxC;++c) {
			out = '.';
			for(i=0;i<num; ++i) {
				if(elf[i].r == r && elf[i].c == c) {
					out = '#';
					break;
				}
			}
			printf("%c", out);
		}
		printf("\n");
	}
}

void simulate() {
	int i, j, nElf, nBuc;
	Elf* near[25];
	Bucket* bucs[4];
	
	buildBuckets();
	
	for(i=0; i<num; ++i) { // prepare move
		nBuc = gatherBuckets(elf + i, 1, bucs);
		assert(nBuc);
		
		nElf = 0;
		for(j=0;j<nBuc; ++j) {
			nElf = gatherElves(elf + i, near, nElf, bucs[j], 1);
		}
		considerMove(elf + i, near, nElf);
	}
	
	//printElves();
	
	for(i=0; i<num; ++i) { // perform move
		nBuc = gatherBuckets(elf + i, 2, bucs);
		assert(nBuc);
		
		nElf = 0;
		for(j=0;j<nBuc; ++j) {
			nElf = gatherElves(elf + i, near, nElf, bucs[j], 2);
		}
		performMove(elf + i, near, nElf);
	}
	
	printf("turn %i\n", turn + 1);
	//printMap();
	
}

int computeAnswer() {
	int tminR, tmaxR, tminC, tmaxC, i;
	tminC = tminR = INT_MAX;
	tmaxC = tmaxR = -INT_MAX;
	
	for(i=0; i<num; ++i) {
		tminR = (tminR > elf[i].r) ? elf[i].r : tminR;
		tmaxR = (tmaxR < elf[i].r) ? elf[i].r : tmaxR;
		tminC = (tminC > elf[i].c) ? elf[i].c : tminC;
		tmaxC = (tmaxC < elf[i].c) ? elf[i].c : tmaxC;
	}
	tmaxC++;
	tmaxR++;
	
	printf("minR %i maxR %i minC %i maxC %i\n", 
		tminR, tmaxR, tminC, tmaxC );
	
	return (tmaxR - tminR) * (tmaxC - tminC) - num;
}

int main(int argc, char** argv) {
	int a;
	memset(bucket, 0, B*B*sizeof(Bucket));
	
	readIt();
	printElves();
	
	while(1) {
		dirty = 0;
		simulate();
		turn++;
		
		if(!dirty) {
			printf("no move on turn %i\n", turn );
			break;
		}
	}
	
	printMap();
	printBuckets();
	a = computeAnswer();
	printf("got %i\n", a);
}
