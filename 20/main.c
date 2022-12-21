#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define INPUT "input.txt"


typedef struct Node {
	long long int v;
	struct Node* next;
	struct Node* prev;
} Node; 

Node data[6000];
int n = 0;

void readIt() {
	FILE* f = fopen(INPUT, "r");
	memset(data, 0, sizeof(Node) * 6000);
	
	while(fscanf(f, "%lli\n", &data[n].v) == 1) {
		data[n].v *= 811589153;
		data[n].next = data + n + 1;
		data[n].prev = data + n - 1;
		n++;
	}
	
	data[0].prev = data + (n - 1);
	data[n - 1].next = data;
	fclose(f);
}

void printIt(Node* n) {
	Node* t = n;
	do {
		printf("%lli,", n->v);
		n = n->next;
	} while(n != t);
	printf("\n");
}

long long int findIt(int at, Node* s) {
	int i;
	Node* t = s;
	while(s->v != 0) s = s->next;
	for(i=0; i<at; ++i) s = s->next;
	return s->v;
}

void testIt(Node* s) {
	int i;
	Node* t;
	
	i = 0; 
	t = s;
	do{
		t=t->prev;
		i++;
	} while(t != s);
	
	printf("got %i\n", i);
	
}

void printIsh(Node* n) {
	int i;
	for(i=0; i<5; ++i) n = n->prev;
	for(i=0; i<10; ++i) {
		printf("%lli,", n->v);
		n = n->next;
	}
	printf("\n");
}

void mixIt() {
	Node *at;
	long long int i, j, back, show;
	
	//printIt(data);
	for(i=0; i<n; ++i) {
		
		at = data + i;
		j = at->v;
		if(j == 0) {
			continue;
		}
		
		// yoink!
		data[i].prev->next = data[i].next;
		data[i].next->prev = data[i].prev;
		
		back = j < 0;
		j = llabs(j);
		j = j % (n - 1);
		
 		while(j--) {
			at = (back) ? at->prev : at->next;
		}
		
		show = 0;
		//if(data + i == at) continue;
		assert(data + i != at );
		
		if(data + i == at->prev) {
			printf("prev hit %lli\n", data[i].v);
			show = 1;
		}
		
		if(data + i == at->next) {
			printf("next hit %lli\n", data[i].v);
			show = 1;
		}
		
		if(show) {
			printIsh(at);
		}

		if(back) {
			data[i].prev = at->prev;
			data[i].next = at;
			at->prev = at->prev->next = data + i;
		} else {
			data[i].next = at->next;
			data[i].prev = at;
			at->next = at->next->prev = data + i;
		}
		
		if(show) {
			printIsh(at);
			printf("\n");
		}
	}
}

int main(int argc, char** argv) {
	long long int a,b,c;
	int i;
	readIt();
	
	for(i=0; i<10; ++i)
		mixIt();
	
	printf("looking for result ...\n");
	a = findIt(1000, data);
	b = findIt(2000, data);
	c = findIt(3000, data);
	
	printf("%lli, %lli, %lli, [%lli]\n", a, b, c, a + b + c);
}
