#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_N    8
#define MAX_CAND (1 << MAX_N)

#define INIT_CANDIDATES {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255}

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

/*
 * find the minimum hamming distance between a codeword and a code (an array of
 * codewords)
 */
uint16_t least_hd(uint16_t* code, uint16_t i, uint16_t word)
{
	uint16_t least = 0xffff;
	uint16_t c;
	while (i) {
		c = *(code + --i);
		if (HD[c][word] < least)
			least = HD[c][word];
	}
	return least;
}

/* If a codeword is at least min_hd away from each in a code, return true. */
bool codeword_vs_code(uint16_t* code, uint16_t i, uint16_t word, int min_hd)
{
	uint16_t c;
	int p = 0;

	while (p < i) {
		c = *(code + p++);

		if (HD[c][word] < min_hd)
			return false;
	}

	return true;
}

/* generate a list of 1<<n integers */
void init_candidates(uint16_t* candidates, uint8_t n)
{
	int i;
	for (i = 0; i < (1<<n); i++)
		*(candidates + i) = (1<<n) - 1 - i;
}

/* recursive search for codes with a given minimum hamming distance */
uint16_t find_codes(uint16_t* code, uint16_t icode, uint16_t* candidates, uint16_t icand, uint8_t min_hd, uint16_t longest)
{
	uint16_t c, nc;
	uint16_t next_candidates[MAX_CAND];
	uint16_t inext_cand;
	int i;
	while (icand) {
		c = *(candidates + --icand);
		*(code + icode++) = c;
		inext_cand = 0;
		for (i = 0; i < icand; i++) {
			nc = *(candidates + i);
			if (HD[c][nc] >= min_hd)
				next_candidates[inext_cand++] = nc;
		}
		/* only look for code at least as long as the longest we have found */
		if ((inext_cand + icode) >= longest) {
			if (inext_cand) {
				longest = find_codes(code, icode, &next_candidates[0], inext_cand, min_hd, longest);
			} else {
				printf("%d [", icode);
				for (i = 0; i < icode; i++)
					printf("%d,", *(code + i));
				printf("]\n");
				if (icode > longest)
					longest = icode;
			}
		}
		icode--;
	}
	return longest;
}

/*
 * Find codes starting with an assumed codeword and a minimum hamming distance.
 */
void find_from_start(uint16_t start, uint8_t n, uint8_t min_hd)
{
	uint16_t code[MAX_CAND];
	uint16_t next_candidates[MAX_CAND];
	static const uint16_t candidates[MAX_CAND] = INIT_CANDIDATES;
	uint16_t icand = 1<<n;
	int i;
	uint16_t nc;
	uint16_t inext_cand = 0;
	code[0] = start;
	for (i = 0; i < icand; i++) {
		nc = *(candidates + i);
		if (HD[start][nc] >= min_hd)
			next_candidates[inext_cand++] = nc;
	}
	find_codes(&code[0], 1, &next_candidates[0], inext_cand, min_hd, 0);
}

/*
 * Find codes of a given codeword length and minimum hamming distance.
 * Actually not all codes because it skips codes shorter than the longest one
 * found so far.
 */
void find_all_codes(uint8_t n, uint8_t min_hd)
{
	uint16_t code[MAX_CAND];
	uint16_t candidates[MAX_CAND] = INIT_CANDIDATES;
	find_codes(&code[0], 0, &candidates[0], 1<<n, min_hd, 0);
}

/* recursive search for codes complementary to a given code */
uint16_t find_comp(uint16_t* a_code, uint16_t ia_code, uint16_t* b_code,
		uint16_t ib_code, uint16_t* candidates, uint16_t icand,
		uint8_t min_hd, uint16_t min_b_len, uint16_t min_len)
{
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
	uint16_t code[MAX_CAND];
	uint16_t next_candidates[MAX_CAND];
	uint16_t next_b_candidates[MAX_CAND];
	static const uint16_t candidates[MAX_CAND] = INIT_CANDIDATES;
	uint16_t icand = 1<<n;
	static const uint16_t b_candidates[MAX_CAND] = INIT_CANDIDATES;
	uint16_t ib_cand = 1<<n;
	int i;
	uint16_t nc, bc;
	uint16_t inext_cand = 0;
	uint16_t inext_b_cand = 0;
	code[0] = start;
	/* populate next_candidates with all codewords min_hd away from start */
	for (i = 0; i < icand; i++) {
		nc = *(candidates + i);
		if (HD[start][nc] >= min_hd)
			next_candidates[inext_cand++] = nc;
	}
	for (i = 0; i < ib_cand; i++) {
		bc = *(b_candidates + i);
		if (HD[start][bc] >= min_iso) {
			next_b_candidates[inext_b_cand++] = bc;
		}
	}
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
