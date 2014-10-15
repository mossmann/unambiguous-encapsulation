#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_N    8
#define MAX_CAND (1 << MAX_N)

const uint8_t HAMMING_WEIGHT[] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8};

uint16_t HD[MAX_CAND][MAX_CAND];

// Here due to my laziness
uint8_t find_longest;

uint16_t hamming_distance(int a, int b)
{
	return HAMMING_WEIGHT[a ^ b];
}

void usage(char *argv0) {
	fprintf(stderr, "%s: [-l] <n> <min_hd> <min_iso> [<a_len>]\n", argv0);
	fprintf(stderr, "\t-l  Find the longest code\n");
	fprintf(stderr, "\tMAX_N = %d\n", MAX_N);
}

/* fill big lookup table of hamming distances */
void precompute_hd()
{
	int i, j;
	for (i = 0; i < MAX_CAND; i++)
		for (j = 0; j < MAX_CAND; j++)
			HD[i][j] = hamming_distance(i, j);
}

void populate_candidates(uint16_t code, uint16_t *candidates,
					 uint16_t* next_candidates, uint16_t icand,
					 uint16_t *inext_cand, uint8_t min_dist)
{
	uint16_t nc;
	int i;
	for (i = 0; i < icand; i++) {
		nc = *(candidates + i);
		if (HD[code][nc] >= min_dist)
			next_candidates[(*inext_cand)++] = nc;
	}
}

/* recursive search for codes complementary to a given code */
uint16_t find_comp(uint16_t* a_code, uint16_t ia_code, uint16_t* b_code,
		uint16_t ib_code, uint16_t* candidates, uint16_t icand,
		uint8_t min_hd, uint16_t min_b_len, uint16_t min_len)
{
	//printf("find_comp(ia_code %d, ib_code %d, icand %d, min_hd %d, min_b_len %d, min_len %d)\n",
	//	   ia_code, ib_code, icand, min_hd, min_b_len, min_len);
	uint16_t c, nc;
	uint16_t next_candidates[MAX_CAND];
	uint16_t inext_cand;
	uint16_t best_len;
	uint16_t longest = 0;
	int i;
	while (icand) {
		c = *(candidates + --icand);
		*(b_code + ib_code++) = c;
		inext_cand = 0;
		populate_candidates(c, candidates, next_candidates,
							icand, &inext_cand, min_hd);

		/* only look for b_code at least as long as the longest we have found */
		if ((inext_cand + ib_code) >= min_b_len) {
			if ((inext_cand + ib_code + ia_code) >= min_len) {
				if (inext_cand) {
					best_len = find_comp(a_code, ia_code, b_code, ib_code,
							&next_candidates[0], inext_cand, min_hd, min_b_len, min_len);
					if (best_len >= min_len) {
						longest = best_len;
						min_len = best_len;
						if(find_longest==1)
							min_len++;
					}
				} else {
					printf("%d %d %d [", ia_code + ib_code, ia_code, ib_code);
					for (i = 0; i < (ia_code - 1); i++)
						printf("%d, ", *(a_code + i));
					printf("%d", *(a_code + ia_code - 1));
					printf("] [");
					for (i = 0; i < (ib_code - 1); i++)
						printf("%d, ", *(b_code + i));
					printf("%d", *(b_code + ib_code - 1));
					printf("]\n");
					if ((ia_code + ib_code) >= min_len)
						longest = ia_code + ib_code;
				}
			}
		}
		ib_code--;
	}
	return longest;
}

/* recursive search for pairs of complementary codes */
uint16_t find_iso(uint16_t* code, uint16_t icode, uint16_t* candidates,
		uint16_t icand, uint16_t* b_candidates, uint16_t ib_cand,
		uint8_t min_hd, uint8_t min_iso, uint16_t a_len,
		uint16_t min_b_len, uint8_t n, uint16_t min_len)
{
	//printf("find_iso icode=%d, icand=%d, ib_cand=%d\n", icode, icand, ib_cand);
	uint16_t c;
	uint16_t next_candidates[MAX_CAND];
	uint16_t next_b_candidates[MAX_CAND];
	uint16_t inext_cand;
	uint16_t inext_b_cand;
	uint16_t max_b_len, total_len;
	uint16_t best_len;
	uint16_t longest = 0;

	while (icand) {
		c = *(candidates + --icand);
		*(code + icode++) = c;
		inext_cand = 0;
		inext_b_cand = 0;
		populate_candidates(c, candidates, next_candidates,
							icand, &inext_cand, min_hd);
		populate_candidates(c, b_candidates, next_b_candidates,
							ib_cand, &inext_b_cand, min_iso);

		if (((inext_cand + icode) >= a_len) && (inext_b_cand >= min_b_len)) {
			if (icode == a_len) {
				uint16_t b_code[MAX_CAND];
				best_len =  find_comp(code, icode, &b_code[0], 0,
									  &next_b_candidates[0], inext_b_cand,
									  min_hd, min_b_len, min_len);
				if (best_len >= min_len) {
					longest = best_len;
					min_len = best_len;
					if(find_longest==1)
						min_len++;
				}
			}
			if ((inext_cand) && (icode < a_len)) {
				best_len = find_iso(code, icode, &next_candidates[0],
						inext_cand, &next_b_candidates[0], inext_b_cand,
						min_hd, min_iso, a_len, min_b_len, n, min_len);
				if (best_len >= min_len) {
					longest = best_len;
					min_len = best_len;
					if(find_longest==1)
						min_len++;
				}
			}
		}
		icode--;
	}
	return longest;
}

/*
 * Starting with an assumed codeword, find pairs of complementary codes with
 * certain characteristics including isolation.
 *
 * start = starting codeword for first code
 * n = length of all codewords in bits
 * min_hd = minimum hamming distance of each code
 * min_iso = minimum isolation between the two codes
 * a_len = number of codewords for first code
 * min_b_len = minimum number of codewords for second code
 */
uint16_t find_iso_from_start(uint16_t start, uint8_t n, uint8_t min_hd,
		uint8_t min_iso, uint16_t a_len, uint16_t min_b_len)
{
	uint16_t code[MAX_CAND];
	uint16_t next_candidates[MAX_CAND];
	uint16_t next_b_candidates[MAX_CAND];
	uint16_t icand = 1<<n;
	uint16_t candidates[icand];
	int i;
	uint16_t nc, bc;
	uint16_t inext_cand = 0;
	uint16_t inext_b_cand = 0;
	code[0] = start;

	// This seems silly, but it helps to reuse the code
	for (i = 0; i < icand; i++)
		candidates[i] = i;

	populate_candidates(start, candidates, next_candidates,
						icand, &inext_cand, min_hd);
	populate_candidates(start, candidates, next_b_candidates,
						icand, &inext_b_cand, min_iso);

	return find_iso(&code[0], 1, &next_candidates[0], inext_cand,
			&next_b_candidates[0], inext_b_cand, min_hd,
			min_iso, a_len, min_b_len, n, a_len + min_b_len);
}

void find_best_iso(uint8_t n, uint8_t min_hd, uint8_t min_iso, uint16_t a_len)
{
	uint16_t min_b_len, longest, longest_b;
	min_b_len = 2;
	longest = a_len * 2;
	
	for (; longest >= (a_len * 2); a_len++) {
		printf("trying a: %d, min b: %d, total: %d\n", a_len,
				min_b_len, a_len + min_b_len);
		longest = find_iso_from_start(0, n, min_hd, min_iso, a_len, min_b_len);
			min_b_len = longest - a_len - 3;
	}
}

int main(int argc, char** argv)
{
	char c;
	find_longest = 0;
	while ( (c = getopt(argc, argv, "l")) != -1) {
		switch (c) {
		case 'l':
			find_longest = 1;
			break;
		}
	}
	if (argc-optind < 3) {
		usage(argv[0]);
		exit(1);
	}
	
	uint8_t n, min_hd, min_iso;
	if ((n 		 = atoi(argv[optind++])) == 0 ||
		(min_hd  = atoi(argv[optind++])) == 0 ||
		(min_iso = atoi(argv[optind++])) == 0) {
		usage(argv[0]);
		exit(1);
	}
	
	int a_len = 2;
	if (argc > optind) {
		a_len = atoi(argv[optind]);
	}
	
	precompute_hd();
	find_best_iso(n, min_hd, min_iso, a_len);
}
