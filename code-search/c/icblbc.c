#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_N    8
#define MAX_CAND (1 << MAX_N)

const uint8_t HAMMING_WEIGHT[] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8};

uint16_t HD[MAX_CAND][MAX_CAND];

uint16_t hamming_distance(a, b)
{
	return HAMMING_WEIGHT[a ^ b];
}

/* fill big lookup table of hamming distances */
void precompute_hd()
{
	int i, j;
	for (i = 0; i < MAX_CAND; i++)
		for (j = 0; j < MAX_CAND; j++)
			HD[i][j] = hamming_distance(i, j);
}

/* recursive search for codes complementary to a given code */
uint16_t find_comp(uint16_t* a_code, uint16_t ia_code, uint16_t* b_code,
		uint16_t ib_code, uint16_t* candidates, uint16_t icand,
		uint8_t min_hd, uint16_t min_b_len, uint16_t min_len)
{
	printf("find_comp(ia_code %d, ib_code %d, icand %d, min_hd %d, min_b_len %d, min_len %d)\n",
		   ia_code, ib_code, icand, min_hd, min_b_len, min_len);
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
		for (i = 0; i < icand; i++) {
			nc = *(candidates + i);
			if (HD[c][nc] >= min_hd)
				next_candidates[inext_cand++] = nc;
		}
		/* only look for b_code at least as long as the longest we have found */
		if ((inext_cand + ib_code) >= min_b_len) {
			if ((inext_cand + ib_code + ia_code) >= min_len) {
				if (inext_cand) {
					best_len = find_comp(a_code, ia_code, b_code, ib_code,
							&next_candidates[0], inext_cand, min_hd, min_b_len, min_len);
					if (best_len >= min_len) {
						longest = best_len;
						min_len = best_len;
					}
				} else {
					printf("%d %d %d [", ia_code + ib_code, ia_code, ib_code);
					for (i = 0; i < (ia_code - 1); i++)
						printf("%d,", *(a_code + i));
					printf("%d", *(a_code + ia_code - 1));
					printf("][");
					for (i = 0; i < (ib_code - 1); i++)
						printf("%d,", *(b_code + i));
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

/* find codes that are complementary to a given code */
uint16_t complementary(uint16_t* a_code, uint16_t ia_code,
		uint16_t* b_candidates, uint16_t ib_cand, uint8_t min_hd,
		uint8_t min_iso, uint16_t min_b_len, uint8_t n, uint16_t min_len)
{
	uint16_t b_code[MAX_CAND];
	return find_comp(a_code, ia_code, &b_code[0], 0, &b_candidates[0],
			ib_cand, min_hd, min_b_len, min_len);
}

/* recursive search for pairs of complementary codes */
uint16_t find_iso(uint16_t* code, uint16_t icode, uint16_t* candidates,
		uint16_t icand, uint16_t* b_candidates, uint16_t ib_cand,
		uint8_t min_hd, uint8_t min_iso, uint16_t a_len,
		uint16_t min_b_len, uint8_t n, uint16_t min_len)
{
	uint16_t c, nc, bc;
	uint16_t next_candidates[MAX_CAND];
	uint16_t next_b_candidates[MAX_CAND];
	uint16_t inext_cand;
	uint16_t inext_b_cand = 0;
	uint16_t max_b_len, total_len;
	uint16_t best_len;
	uint16_t longest = 0;
	int i;
	while (icand) {
		c = *(candidates + --icand);
		*(code + icode++) = c;
		inext_cand = 0;
		inext_b_cand = 0;
		for (i = 0; i < icand; i++) {
			nc = *(candidates + i);
			if (HD[c][nc] >= min_hd)
				next_candidates[inext_cand++] = nc;
		}
		for (i = 0; i < ib_cand; i++) {
			bc = *(b_candidates + i);
			if (HD[c][bc] >= min_iso)
				next_b_candidates[inext_b_cand++] = bc;
		}
		if (((inext_cand + icode) >= a_len) && (inext_b_cand >= min_b_len)) {
			if (icode == a_len) {
				best_len = complementary(code, icode, &next_b_candidates[0],
						inext_b_cand, min_hd, min_iso, min_b_len, n, min_len);
				if (best_len >= min_len) {
					longest = best_len;
					min_len = best_len;
				}
			}
			if ((inext_cand) && (icode < a_len)) {
				best_len = find_iso(code, icode, &next_candidates[0],
						inext_cand, &next_b_candidates[0], inext_b_cand,
						min_hd, min_iso, a_len, min_b_len, n, min_len);
				if (best_len >= min_len) {
					longest = best_len;
					min_len = best_len;
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
	printf("find_iso_from_start(start %d, n %d, min_hd %d, min_iso %d, a_len %d, min_b_len %d)\n",
		   start, n, min_hd, min_iso, a_len, min_b_len);
	uint16_t code[MAX_CAND];
	uint16_t next_candidates[MAX_CAND];
	uint16_t next_b_candidates[MAX_CAND];
	uint16_t icand = 1<<n;
	int i;
	uint16_t nc, bc;
	uint16_t inext_cand = 0;
	uint16_t inext_b_cand = 0;
	code[0] = start;
	/* populate next_candidates with all codewords min_hd away from start */
	for (i = 0; i < icand; i++) {
		if (HD[start][i] >= min_hd) {
			next_candidates[inext_cand++] = i;
			if (HD[start][i] >= min_iso)
				next_b_candidates[inext_b_cand++] = i;
		}
	}

	printf("inext_cand=%d, inext_b_cand=%d\n", inext_cand, inext_b_cand);
	return find_iso(&code[0], 1, &next_candidates[0], inext_cand,
			&next_b_candidates[0], inext_b_cand, min_hd,
			min_iso, a_len, min_b_len, n, a_len + min_b_len);
}

void find_best_iso(uint8_t n, uint8_t min_hd, uint8_t min_iso)
{
	uint16_t min_b_len = 2;
	uint16_t a_len;
	uint16_t longest, longest_b;

	for (a_len = 1<<(n-1); a_len >= min_b_len; a_len--) {
		printf("trying a: %d, min b: %d, total: %d\n", a_len,
				min_b_len, a_len + min_b_len);
		longest = find_iso_from_start(0, n, min_hd, min_iso, a_len, min_b_len);
		if (longest >= (a_len + min_b_len)) {
			min_b_len = longest - a_len + 1;
		}

	}
}

int main(void)
{
	precompute_hd();
	//find_all_codes(8, 4);
	//find_from_start(0, 6, 4);
	//find_iso_from_start(0, 7, 2, 3, 60, 2);
	//find_iso_from_start(0, 5, 2, 3, 4, 4);
	find_best_iso(6, 2, 3);
}
