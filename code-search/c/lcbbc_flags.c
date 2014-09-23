#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int cand_len;
int max_cand;
uint8_t *HD;

/* fill big lookup table of hamming distances */
void precompute_hd(int min_hd)
{
	int i, j, k;
	uint8_t *current;
	uint16_t value;
	
	HD = (uint8_t *) calloc(max_cand, cand_len);
	if(HD == NULL) {
		fprintf(stderr, "Error: unable to allocate memory for hamming distance table\n");
		exit(1);
	}
	
	for (i = 0; i < max_cand; i++) {
		current =  HD + i*cand_len;
		for (j = 0; j < cand_len; j++) {
			for(k=7;k>=0;k--) {
				current[j] <<= 1;
				value = (j*8 + k);
				if(__builtin_popcount(i ^ value) >= min_hd)
					current[j] |= 0x01;
			}
		}
	}
}

typedef struct code_list_t {
	uint32_t index;
	uint16_t *codewords;
} code_list;

typedef struct codeword_list_t {
	uint32_t size;
	uint8_t *codewords;
} codeword_list;

codeword_list *new_codeword_list() {
	codeword_list *list;
	list = (codeword_list *) malloc(sizeof(codeword_list));
	if(list == NULL)
		fprintf(stderr, "Malloc failed to allocate for codeword_list!\n");
	list->codewords = (uint8_t *) calloc(1, cand_len);
	if(list->codewords == NULL)
		fprintf(stderr, "Malloc failed to allocate for list->codewords!\n");
	return list;
}

/* When converted to flags in an int, this can be impemented as an AND operation */
void populate_candidates(uint16_t code, codeword_list *candidates,
						 codeword_list* next_candidates)
{
	int i;
	uint8_t *byte = HD + code*cand_len;
	next_candidates->size = 0;
	for (i = 0; i < cand_len; i++) {
		next_candidates->codewords[i] = candidates->codewords[i] &  byte[i];
		next_candidates->size += __builtin_popcount(next_candidates->codewords[i]);
	}
}

uint16_t next_bit(uint8_t *array) {
	uint8_t c, x;
	for(x=0; x<cand_len; x++) {
		if(array[x] != 0) {
			for(c=0; c<8; c++) {
				if(array[x] & 1<<c) {
					array[x] ^= 1 << c;
					return 8*x + c;
				}
			}
		}
	}
	return 0;
}

/* recursive search for longest codes */
int find_best_code(code_list* code, codeword_list* candidates, int longest)
{
	codeword_list *next_candidates;
	uint16_t c;

	next_candidates = new_codeword_list();
	while (candidates->size) {
		c = next_bit(candidates->codewords);
		code->codewords[code->index++] = c;
		candidates->size--;
		populate_candidates(c, candidates, next_candidates);
	
		if ((next_candidates->size + code->index) > longest) {
			if(next_candidates->size) {
				longest = find_best_code(code, next_candidates, longest);
			}
			else {
				longest = code->index;
				printf("%d, [", longest);
				int i;
				for(i=0; i<longest-1; i++)
					printf("%d, ", code->codewords[i]);
				printf("%d]\n", code->codewords[longest-1]);
			}
		}
		code->index--;
	}
	free(next_candidates->codewords);
	free(next_candidates);
	
	return longest;
}

int find_code() {
	int i, longest;
	code_list *code;
	codeword_list *candidates, *next_candidates;
	code = malloc(sizeof(code_list));
	code->codewords = malloc(max_cand);
	code->index = 1;
	code->codewords[0] = 0;
	
	candidates = new_codeword_list();
	for (i = 0; i < cand_len; i++)
		candidates->codewords[i] = 0xff;
	candidates->codewords[cand_len-1] >>= (max_cand%8);
	candidates->codewords[0] ^= 0x01;
	
	next_candidates = new_codeword_list();
	populate_candidates(0, candidates, next_candidates);
	longest = find_best_code(code, next_candidates, 0);
	
	free(candidates->codewords);
	free(candidates);
	free(next_candidates->codewords);
	free(next_candidates);
	free(code->codewords);
	free(code);
	
	return longest;
}

void usage(char *argv0) {
	fprintf(stderr, "%s: <n> <min_hd>\n", argv0);
}

int main(int argc, char** argv)
{
	if (argc < 3) {
		usage(argv[0]);
		exit(1);
	}

	int min_hd, n;
	if ((n 		 = atoi(argv[1])) == 0 ||
		(min_hd  = atoi(argv[2])) == 0) {
		usage(argv[0]);
		exit(1);
	}

	max_cand = 1 << n;
	cand_len = (max_cand+7) / 8;

	precompute_hd(min_hd);
	find_code();
	
	free(HD);
	return 0;
}
