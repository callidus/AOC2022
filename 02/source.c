
#include <stdio.h>
#include <stdlib.h>

int readIt(char** iptA, char** iptB) {
	FILE* f;
	char* bufA, * bufB, c, d;
	int numA, numB, szA, szB;

	bufA = malloc(szA = 10);
	bufB = malloc(szB = 10);
	c = d = numA = numB = 0;
	f = fopen("input2.txt", "r");

	while (!feof(f)) {
		if (fscanf(f, "%c %c\n", &c, &d)) {
			bufA = (numA == szA) ? realloc(bufA, szA *= 2) : bufA;
			bufA[numA++] = c;
			bufB = (numB == szB) ? realloc(bufB, szB *= 2) : bufB;
			bufB[numB++] = d;
		}
	}
	
	fclose(f);

	*iptA = bufA;
	*iptB = bufB;
	return numA;
}

// solution part 1
//   r  p   s -              <- me
int lut1[3][3] = {
	{4, 8, 3},   /// r 
	{1, 5, 9},   /// p
	{7, 2, 6},   /// s
};

// solution part 2
//   l  d   w -              <- me
int lut2[3][3] = {
	{3, 4, 8},   /// r 
	{1, 5, 9},   /// p
	{2, 6, 7},   /// s
};

int main(int argc, char** argv) {
	char *iptA, *iptB;
	int num, score, i;

	num = readIt(&iptA, &iptB);

	score = 0;
	for (i = 0; i < num; ++i) {
		score += lut2[iptA[i] - 65][iptB[i] - 88];
	}

	free(iptA);
	free(iptB);
	
	printf("score %i\n", score);
	return 0;
}
