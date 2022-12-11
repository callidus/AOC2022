
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// the reason this works for part 2 is here:
// https://en.wikipedia.org/wiki/Chinese_remainder_theorem 

typedef struct {
	unsigned long long int* items;
	int num, sz, opVal, div, toA, toB, count;
	char op;
} Monkey;

Monkey monkeys[10];
int numMonkeys = 0;
int maxDiv = 1;

void addItem(Monkey* m, unsigned long long int item) {
	m->items = (m->num == m->sz) ? realloc(m->items, (m->sz += 10) * sizeof(unsigned long long int)) : m->items;
	m->items[m->num++] = item;
}

void parseMonkey(FILE* f) {
	char buf[8];
	int tmp;
	Monkey* m;

	fscanf(f, "Monkey %i:\n", &tmp);
	m = monkeys + tmp;
	memset(m, 0, sizeof(Monkey));
	
	fscanf(f, "  Starting items:");
	while (fscanf(f, " %i", &tmp)) {
		addItem(m, tmp);
		fgetc(f);
	}

	m->opVal = -1;
	fscanf(f, "  Operation: new = old %c %s\n", &m->op, buf);
	if (buf[0] > 47 && buf[0] < 58) {
		m->opVal = atoi(buf);
	}

	fscanf(f, "  Test: divisible by %i\n", &m->div);
	fscanf(f, "    If true: throw to monkey %i\n", &m->toA);
	fscanf(f, "    If false: throw to monkey %i\n\n", &m->toB);
}

void printMonkey(Monkey* m) {
	int i;
	printf("%i (op %c %i) %i [%i %i] Items: ", m->count, m->op, m->opVal, m->div, m->toA, m->toB);
	for (i = 0; i < m->num; ++i) {
		printf("%llu ", m->items[i]);
	}
	printf("\n");
	
}

void simulate() {
	int i, j, to;
	unsigned long long int tmp, opVal;
	for (i = 0; i < numMonkeys; ++i) {
		for (j = 0; j < monkeys[i].num; ++j) {
			opVal = monkeys[i].opVal < 0 ? monkeys[i].items[j] : monkeys[i].opVal;
			tmp = (monkeys[i].op == '*' ? monkeys[i].items[j] * opVal : monkeys[i].items[j] + opVal) % maxDiv;
			to = (tmp % monkeys[i].div == 0) ? monkeys[i].toA : monkeys[i].toB;
			addItem(monkeys + to, tmp);
			monkeys[i].count++;
		}
		monkeys[i].num = 0;
	}
}

int main(int argc, char** argv) {
	int i;
	unsigned long long int bA, bB;
	FILE* f;

	
	f = fopen("input.txt", "r");
	while (!feof(f)) {
		parseMonkey(f);
		numMonkeys++;
	}

	for (i = 0; i < numMonkeys; ++i) {
		printMonkey(monkeys + i);
		// product of all divisors, mod by this wont change results, see wiki link at top of file 
		maxDiv *= monkeys[i].div;
	}

	for (i = 0; i < 10000; ++i) {
		simulate();
	}

	printf("================\n");

	bA = bB = 0;
	for (i = 0; i < numMonkeys; ++i) {
		printMonkey(monkeys + i);
		if (monkeys[i].count > bA) {
			bA = monkeys[i].count;
		}
	}

	for (i = 0; i < numMonkeys; ++i) {
		if (monkeys[i].count > bB && monkeys[i].count < bA) {
			bB = monkeys[i].count;
		}
	}
	printf("%llu %llu %llu\n", bA, bB, bA * bB);
}
