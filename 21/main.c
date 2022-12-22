#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <math.h>

#define INPUT "input.txt"


typedef struct Node {
	struct Node *lhs, *rhs, *par;
	char lhsn[5], rhsn[5];
	long long int val;
	char name[5];
	char op, dirty;
} Node; 

Node* data;
int num = 0;
int max = 0;

void pushNode(Node* n) {
	if(num == max) {
		max += 10;
		data = realloc(data, max * sizeof(Node));
	}
	data[num++] = *n;
}

void readIt() {
	int i, j;
	char c;
	Node node;
	FILE* f = fopen(INPUT, "r");
	
	while(!feof(f)) {
		memset(&node, 0, sizeof(Node));
		for(i=0; i<4; ++i) 
			node.name[i] = fgetc(f);
		assert(fgetc(f) == ':');
		assert(fgetc(f) == ' ');
		
		c = fgetc(f); 
		ungetc(c, f);
		if( c < 48 || c > 57 ) {
			fscanf(f, "%s %c %s\n", node.lhsn, &node.op, node.rhsn);
			printf("got op %s %c %s\n", node.lhsn, node.op, node.rhsn);
		} else {
			fscanf(f, "%lli\n", &node.val);
			printf("got val %lli\n", node.val);
		}
		node.dirty = 1;
		pushNode(&node);
	}

	fclose(f);

	// monkeys live in trees!
	for(i=0; i<num; ++i) {
		if(data[i].op != 0) {
			
			for(j=0; j<num; ++j) {
				if(strcmp(data[i].lhsn, data[j].name) == 0) {
					data[i].lhs = data + j;
					data[i].lhs->par = data + i;
					break;
				}
			}
			
			for(j=0; j<num; ++j) {
				if(strcmp(data[i].rhsn, data[j].name) == 0) {
					data[i].rhs = data + j;
					data[i].lhs->par = data + i;
					break;
				}
			}

		}
	}
}

// have stack, will rec!
long long int computeIt(Node* n) {
	if(n->dirty) {
		switch(n->op) {
			case '+': n->val = computeIt(n->lhs) + computeIt(n->rhs); break;
			case '-': n->val = computeIt(n->lhs) - computeIt(n->rhs); break;
			case '*': n->val = computeIt(n->lhs) * computeIt(n->rhs); break;
			case '/': n->val = computeIt(n->lhs) / computeIt(n->rhs); break;
			case '=': {
				long long int l = computeIt(n->lhs);
				long long int r = computeIt(n->rhs);
				long long int t = r - l;
				printf("got %lli\n", t);
				//exit(0);
				if(l < r) return -1;
				if(l > r) return 1;
				return 0;
			}
			default: break;
		}
	}
	return n->val;
}

Node* findIt(const char* name) {
	int i; 
	for(i=0; i<num; ++i) {
		if(strcmp(name, data[i].name) == 0)
			return data + i;
	}
}

void updateIt(Node* n) {
	n->dirty = 1;
	if(n->par) {
		updateIt(n->par);
	}
}

int main(int argc, char** argv) {
	int t, i, p;
	long long int f;
	Node* humn, *root;
	readIt();
	humn = findIt("humn");
	root = findIt("root");
	
	root->op = '=';
	humn->val = 3882224466195; // vague start point, close enough
	printf("try %lli\n", humn->val);
	updateIt(humn);
	
	
	/*
	 * add powers of two until too high, then subtract the 
	 * next lowest power of 2 until too low. Keep adding then subtracting 
	 * smaller and smaller numbers, oscilating either side of, but getting
	 * closer to the right value. Eventually we land on it.
	 * NOTE: this can find a solution that works but is too high because of
	 * int overflow. If that happens adjust the start point down a bit and
	 * try again.
	 */
	f=t=p=1;
	for(i=30; i>=1 && t!=0; --i) {
		while((t = computeIt(root)) == p) {
			humn->val += (pow(2,i) * f);
			updateIt(humn);
			p = t;
		}
		if(t==0) break;
		f *= -1;
		p = t;
	}
	
	printf("yell %lli\n", humn->val);
}

