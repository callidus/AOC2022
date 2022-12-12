
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct Node {
	int val, f, g, h, x, y;
	struct Node* p;
};

typedef struct Node Node;

#define D

#ifdef D
#define IPT "input.txt"
#define W 70
#define H 41

#else
#define IPT "input2.txt"
#define W 8
#define H 5

#endif

Node buf[W][H];
Node *end, *start;

int numOpen = 0;
int numClosed = 0;
Node* closedSet[W * H];
Node* openSet[W * H];

void readIt() {
	int x, y;
	FILE* f;

	end = start = NULL;
	memset(buf, 0, sizeof(Node) * W * H);
	f = fopen(IPT, "r");
	
	for (y = 0; y < H; ++y) {
		for (x = 0; x < W; ++x) {
			if ((buf[x][y].val = fgetc(f)) == 'E') {
				buf[x][y].val = 'z';
				end = &buf[x][y];
			} else if (buf[x][y].val == 'S') {
				buf[x][y].val = 'a';
				start = &buf[x][y];
			}
			buf[x][y].x = x;
			buf[x][y].y = y;
		}
		fgetc(f);
	}
	fclose(f);
}

void delInSet(Node** set, int num, int at) {
	if (at != num - 1) {
		memmove(set + at, set + at + 1, (num - (at + 1)) * sizeof(Node*));
	}
}

int findInSet(Node* n, const Node** l, int num) {
	int i;
	for (i=0; i < num; ++i) {
		if (n == l[i])
			return i;
	}
	return -1;
}

Node* findBest() {
	Node* best;
	int i, j = 0;
	
	best = openSet[0];
	for (i = 1; i < numOpen; ++i) {
		if (openSet[i]->f < best->f) {
			best = openSet[i];
			j = i;
		}
	}

	delInSet(openSet, numOpen, j);
	numOpen--;

	return best;
}

int trimSet(int f, Node** l, Node* n, int* num) {
	int at = findInSet(n, l, *num);
	if (at >= 0) {
		if (l[at]->f <= f) {
			return 1;
		}

		delInSet(l, *num, at);
		(*num)--;
	}

	return 0;
}
void printSet(Node** l, int num) {
	int i;
	printf("[");
	for (i = 0; i < num; ++i) {
		printf("(%i %i), ", l[i]->x, l[i]->y);
	}
	printf("] %i\n", num);
}


void printStuff() {
	int x, y;
	for (y = 0; y < H; ++y) {
		for (x = 0; x < W; ++x) {
			printf("%c", buf[x][y].val);
		}
		printf("\n");
	}
}

int findRoute(Node* s, Node* e) {
	int i, g, h, f;
	Node* n, *c[4];

	s->g = 0;
	s->f = s->h = ((s->x - e->x) * (s->x - e->x)) + ((s->y - e->y) * (s->y - e->y)); // heuristic
	openSet[numOpen++] = s;
	
	while (numOpen) {
		if ((n = findBest()) == e) {
			break;
		}

		assert(n->val < 128);
		c[0] = n->x < W - 1 ? &buf[n->x + 1][n->y] : NULL;
		c[1] = n->x > 0 ? &buf[n->x - 1][n->y] : NULL;
		c[2] = n->y < H - 1 ? &buf[n->x][n->y + 1] : NULL;
		c[3] = n->y > 0 ? &buf[n->x][n->y - 1] : NULL;

		g = n->g + 10;
		for (i = 0; i < 4; ++i) {
			//printf("n %i %i  (num open %i num closed %i)\n", n->x, n->y, numOpen, numClosed);

			if (c[i] && c[i]->val <= n->val + 1) { // not too high
				h = ((c[i]->x - e->x) * (c[i]->x - e->x)) + ((c[i]->y - e->y) * (c[i]->y - e->y)); // heuristic
				f = g + h;

				if (trimSet(f, openSet, c[i], &numOpen)) {
					continue;
				}

				if (trimSet(f, closedSet, c[i], &numClosed)) {
					continue;
				}

				c[i]->p = n;
				c[i]->g = g;
				c[i]->h = h;
				c[i]->f = f;

				assert(numOpen < W* H);
				openSet[numOpen++] = c[i];

			}
		}
		assert(numClosed < W* H);
		closedSet[numClosed++] = n;
	}

	i = 0;
	n = e;
	while (n && n != s) {
		n = n->p;
		++i;
	}
	
	return n ? i : INT_MAX;
}

void reset() {
	int x, y;
	numOpen = numClosed = 0;
	for (y = 0; y < H; ++y) {
		for (x = 0; x < W; ++x) {
			buf[x][y].p = NULL;
		}
	}
}

int main(int argc, char** argv) {
	int x, y, i, best;
	Node* n;

	
	readIt();
	printStuff();

	best = INT_MAX;
	for (y = 0; y < H; ++y) {
		for (x = 0; x < W; ++x) {
			n = &buf[x][y];
			if (n->val == 'a') {
				reset();
				i = findRoute(n, end);
				if (i < best) {
					best = i;
				}
			}
		}
	}
	
	printf("in %i\n", best);
}
