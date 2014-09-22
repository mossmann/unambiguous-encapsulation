#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "TRACE.h"

#define MAX_N    8
#define MAX_CAND (1 << MAX_N)

uint8_t HD[MAX_CAND][MAX_CAND];

void usage(char *argv0) {
	fprintf(stderr, "%s: <n> <min_hd>\n", argv0);
	fprintf(stderr, "    MAX_N = %d\n", MAX_N);
}

/* fill big lookup table of hamming distances */
void precompute_hd(int min_hd)
{
	int i, j;
	for (i = 0; i < MAX_CAND; i++)
		for (j = 0; j < MAX_CAND; j++)
			/* FIXME store as bits in bytes and
			 * use popcount to remove MAX_N limitation
			 */
			if(__builtin_popcount(i ^ j) >= min_hd)
				HD[i][j] = 1;
			else
				HD[i][j] = 0;
}

typedef struct codeword_list_t {
	uint16_t index;
	uint16_t codewords[MAX_CAND];
} codeword_list;

void populate_candidates(uint16_t code, codeword_list *candidates,
						 codeword_list* next_candidates)
{
	uint16_t nc;
	int i;
	for (i = 0; i < candidates->index; i++) {
		nc = candidates->codewords[i];
		if (HD[code][nc])
			next_candidates->codewords[next_candidates->index++] = nc;
	}
}

/* recursive search for longest codes */
int find_best_code(codeword_list* code, codeword_list* candidates, int longest)
{
	codeword_list next_candidates;
	uint16_t c;

	while (candidates->index) {
		candidates->index--;
		c = candidates->codewords[candidates->index];
		code->codewords[code->index++] = c;
		next_candidates.index = 0;
		populate_candidates(c, candidates, &next_candidates);

		if ((next_candidates.index + code->index) > longest) {
			if(next_candidates.index)
				longest = find_best_code(code, &next_candidates, longest);
			else {
				longest = code->index;
				printf("%d, [", longest);
				int i;
				for(i=0; i<longest; i++)
					printf("%d, ", code->codewords[i]);
				printf("]\n");
			}
		}
		code->index--;
	}
	return longest;
}

int find_code(int bitlength) {
	int i;
	codeword_list code, candidates;
	code.index = 1;
	code.codewords[0] = 0;
	candidates.index = 1 << bitlength;
	for (i = 1; i <= candidates.index; i++)
		candidates.codewords[i] = candidates.index - i;
	
	codeword_list next_candidates;
	next_candidates.index = 0;
	populate_candidates(0, &candidates, &next_candidates);
	return find_best_code(&code, &next_candidates, 0);
	
}

int main(int argc, char** argv)
{
	if (argc < 3) {
		usage(argv[0]);
		exit(1);
	}

	int n, min_hd;
	if ((n 		 = atoi(argv[1])) == 0 ||
		(min_hd  = atoi(argv[2])) == 0 ||
		n > MAX_N) {
		usage(argv[0]);
		exit(1);
	}

	precompute_hd(min_hd);
	find_code(n);

	return 0;
}
