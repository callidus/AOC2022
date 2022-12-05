
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct {
	int num;
	int sz;
	char* data;
} Stack;

Stack stacks[9];

void pushn(int stack, const char* val, int num) {
	Stack* s;

	s = stacks + stack;
	s->data = s->sz - s->num <= num ? realloc(s->data, s->sz += (num + 10)) : s->data;
	memcpy(s->data + s->num, val, num);
	s->num += num;
}

void push(int stack, char val){
	Stack* s;

	s = stacks + stack;
	s->data = s->num == s->sz ? realloc(s->data, s->sz += 10) : s->data;
	s->data[s->num++] = val;
}

char pop(int stack){
	Stack* s = stacks + stack;
	assert(s->num);
	return s->data[--s->num];
}

// /*
const int N = 9;
const char* ipt = "input.txt";
const char* data[] = {
	"SMRNWJVT",
	"BWDJQPCV",
	"BJFHDRP",
	"FRPBMND",
	"HVRPTB",
	"CBPT",
	"BJRPL",
	"NCSLTZBW",
	"LSG",
};
// */

 /* example data
const int N = 3;
const char* ipt = "input2.txt";
const char* data[] = {
	"ZN",
	"MCD",
	"P",
};
// */

int main(int argc, char** argv) {
	int i, num, from, to;
	FILE* f;
	memset(stacks, 0, sizeof(Stack) * 9);

	for (i = 0; i < N; ++i) { // populate start data
		stacks[i].num = stacks[i].sz = strlen(data[i]);
		stacks[i].data = malloc(stacks[i].sz);
		memcpy(stacks[i].data, data[i], stacks[i].sz);
	}

	f = fopen(ipt, "r");
	while (!feof(f)) {
		if (fscanf(f, "move %i from %i to %i\n", &num, &from, &to)) {
			to--;
			from--;

			/* part 1
			for (i = 0; i < num; ++i) {
				push(to, pop(from));
			}
			*/

			// part 2
			pushn(to, stacks[from].data + stacks[from].num - num, num);
			stacks[from].num -= num;
		}
	}

	for (i = 0; i < N; ++i) {
		printf("%i: [%c] (%.*s)\n", i, stacks[i].data[stacks[i].num - 1], stacks[i].num, stacks[i].data);
		free(stacks[i].data);
	}
	return 0;
}


// BRQWDBBJN