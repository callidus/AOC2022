
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct INode {
	int bytes;
	struct INode** subs;
	int num;
	int sz;
};

typedef struct INode INode;

char buf[128];

/*
* irelevent for solution:
*  we dont care about 'dir', 
*  we dont care about names
*  we dont care about '$ ls'
*/

void addSub(INode * a, INode* b) {
	a->subs = (a->num == a->sz) ? realloc(a->subs, a->sz + 10) : a->subs;
	a->subs[a->num++] = b;
}

INode* parseDirs(FILE* f, INode* n) {
	char c;
	int sz;
	memset(n, 0, sizeof(INode));

	while (!feof(f)) {
		switch ((c = fgetc(f))) {
		case '$': // command
			memset(buf, 0, 128);
			if (fscanf(f, " cd %s\n", buf)) {
				if (strcmp("..", buf) == 0) {
					return n;
				} else {
					addSub(n, parseDirs(f, malloc(sizeof(INode))));
				}
			} else { // some other stuff, we dont care
				while (fgetc(f) != '\n');
			}
			break;

		case 'd': // dir, we dont care
			while (fgetc(f) != '\n');
			break;

		default: // file, just add its size
			ungetc(c, f);
			fscanf(f, "%i %s\n", &sz, buf);
			n->bytes += sz;
			break;
		}
	}

	return n;
}

void sumDirs(INode* n) {
	int i;
	for (i = 0; i < n->num; ++i) {
		sumDirs(n->subs[i]);
		n->bytes += n->subs[i]->bytes;
	}
}

void dumpDirs(INode* n, int d) {
	int i;
	printf("%.*s %i\n", d, " ", n->bytes);
	for (i = 0; i < n->num; ++i) {
		dumpDirs(n->subs[i], d + 1);
	}
}

void findStuff(INode* n, int* sz) {
	int i;
	if (n->bytes <= 100000) *sz += n->bytes;
	for (i = 0; i < n->num; ++i) {
		findStuff(n->subs[i], sz);
	}
}

int killStuff(INode* n, int need, int best) {
	int i;
	if (n->bytes > need && n->bytes < best) best = n->bytes;

	for (i = 0; i < n->num; ++i) {
		best = killStuff(n->subs[i], need, best);
	}
	return best;
}

int main(int argc, char** argv) {
	FILE* f;
	int sz;
	INode* root = malloc(sizeof(INode));

	f = fopen("input.txt", "r");
	fscanf(f, "$ cd /\n"); /* trim it */
	root = parseDirs(f, root);
	sumDirs(root);
	dumpDirs(root, 0);

	sz = 0;
	findStuff(root, &sz);

	printf("-> %i\n", sz);
	printf("free %i\n", (70000000 - root->bytes));
	printf("need %i\n", 30000000 - (70000000 - root->bytes));

	sz = killStuff(root, 30000000 - (70000000 - root->bytes), INT_MAX);
	printf("killed %i\n", sz);
	
	return 0;
}
