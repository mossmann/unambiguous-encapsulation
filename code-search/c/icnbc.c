#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

#define ALPHABET_LEN 7

uint8_t LD[ALPHABET_LEN][ALPHABET_LEN];

// Here due to my laziness
uint8_t find_longest;

void usage(char *argv0) {
	fprintf(stderr, "%s: [-l] <n> <min_ld> <min_iso> [<a_len>]\n", argv0);
	fprintf(stderr, "\t-l  Find the longest code\n");
}

typedef struct codeword_list_t {
	uint32_t index;
	uint8_t *codewords;
} codeword_list;

codeword_list *new_codeword_list(uint8_t n, int size) {
	codeword_list *cw_list;
	cw_list = (codeword_list*) malloc(sizeof(cw_list));
	if (cw_list==NULL)
		exit(1);
	cw_list->index = 0;
	cw_list->codewords = (uint8_t*) malloc(size * n);
	if (cw_list->codewords==NULL)
		exit(1);
	return cw_list;
}

void delete_codeword_list(codeword_list *list) {
	free(list->codewords);
	free(list);
}

void copy_codeword(uint8_t *src, int src_offset,
								 uint8_t *dst, int dst_offset,
								 int n) {
	int i;
	uint8_t *from, *to;
	from = src + src_offset * n;
	to = dst + dst_offset * n;
	for(i=0; i<n; i++) {
		to[i] = from[i];
	}
}

void print_code(codeword_list *a_code, codeword_list *b_code, uint8_t n) {
	int i,j,k;
	codeword_list *code, **ptr, *code_ptrs[] = {a_code, b_code, NULL};
	ptr = code_ptrs;
	
	if (b_code)
		printf("%d %d %d", a_code->index + b_code->index, a_code->index, b_code->index);
	else
		printf("%d", a_code->index);
	
	while(code = *ptr++) {
		printf(" [");
		for(i=0; i<code->index; i++) {
			j = i*n;
			printf("(");
			for(k=j; k<j+n-1; k++)
				printf("%d, ", code->codewords[k]);
			printf("%d)", code->codewords[k]);
			if(i!=code->index-1)
				printf(", ");
		}
		printf("]");
	}
	printf("\n");
}

int lee_distance(uint8_t* x, uint8_t *y, uint8_t n) {
	int i, ld = 0;
	uint8_t diff;
	for(i=0; i<n; i++) {
		diff = abs(x[i]-y[i]);
		ld += MIN(diff, ALPHABET_LEN-diff);
	}
	return ld;
}

void precompute_ld() {
	int i, j;
	uint8_t diff;
	for(i=0; i<ALPHABET_LEN; i++) {
		for(j=0; j<ALPHABET_LEN; j++) {
			diff = abs(i-j);
			LD[i][j] = MIN(diff, ALPHABET_LEN-diff);
		}
	}
}

int quick_lee_distance(uint8_t* x, uint8_t *y, uint8_t n) {
	int i, ld = 0;
	for(i=0; i<n; i++) {
		ld += LD[x[i]][y[i]];
	}
	return ld;
}

codeword_list* populate_candidates(codeword_list* code,
								   codeword_list *candidates,
								   uint8_t n, uint8_t min_dist) {
	codeword_list* next_candidates;
	int i;
	next_candidates = new_codeword_list(n, candidates->index);
	next_candidates->index = 0;
	uint8_t *codeword = code->codewords + (code->index-1)*n;
	for (i=0; i<candidates->index; i++) {
		if (quick_lee_distance(candidates->codewords + n*i, codeword, n) >= min_dist) {
			copy_codeword(candidates->codewords, i, next_candidates->codewords,
						  next_candidates->index++, n);
		}
	}
	return next_candidates;
}

codeword_list *create_search_space(uint8_t n) {
	int size, i, j, k;
	codeword_list *space;
	
	size = pow(ALPHABET_LEN, n);
	space = new_codeword_list(n, size);
	space->index = size;
	
	j = (size-1)*n;
	for(i=0; i<n; i++)
		space->codewords[i+j] = 0;
	
	for(i=size-2; i>=0; i--) {
		j = i*n;
		copy_codeword(space->codewords, i+1, space->codewords, i, n);
		
		for(k=n-1; k>=0; k--) {
			space->codewords[j+k] = (space->codewords[j+k] + 1) % ALPHABET_LEN;
			if(space->codewords[j+k])
				break;
		}
	}
	return space;
}

uint16_t find_comp(codeword_list *a_code, codeword_list *b_code,
				   codeword_list* candidates, uint8_t n, uint8_t min_ld,
				   uint16_t min_b_len, uint16_t min_len) {
	codeword_list *next_candidates;
	uint16_t best_len, longest = 0;
	
	while (candidates->index--) {
		copy_codeword(candidates->codewords, candidates->index,
					  b_code->codewords, b_code->index++, n);
		
		next_candidates = populate_candidates(b_code, candidates, n, min_ld);
		
		/* only look for b_code at least as long as the longest we have found */
		if ((candidates->index + b_code->index) >= min_b_len) {
			if ((candidates->index + b_code->index + a_code->index) >= min_len) {
				if (candidates->index) {
					best_len = find_comp(a_code, b_code, next_candidates, n,
										 min_ld, min_b_len, min_len);
					if (best_len >= min_len) {
						longest = best_len;
						min_len = best_len;
						if(find_longest==1)
							min_len++;
					}
				} else {
					print_code(a_code, b_code, n);
					if ((a_code->index + b_code->index) >= min_len)
						longest = a_code->index + b_code->index;
				}
			}
		}
		delete_codeword_list(next_candidates);
		b_code->index--;
	}
	return longest;
}

int is_duplicate(uint8_t *codeword, int n) {
	int i;
	for(i=1; i<n; i++)
		if(codeword[i] < codeword[i-1])
			return 1;
	return 0;
}

int find_iso(codeword_list *code, codeword_list *candidates,
			 codeword_list *b_candidates, uint8_t n, uint8_t min_ld,
			 uint8_t min_iso, uint16_t a_len, uint16_t min_b_len,
			 uint16_t min_len) {
	codeword_list *next_candidates, *next_b_candidates;
	uint16_t max_b_len, total_len, best_len, longest = 0;
	int i, j;
	
	while (candidates->index--) {
		if(code->index == 1) {
			if (is_duplicate(candidates->codewords + candidates->index*n, n))
				continue;
		}
		copy_codeword(candidates->codewords, candidates->index,
					  code->codewords, code->index++, n);
		
		next_candidates = populate_candidates(code, candidates, n, min_ld);
		next_b_candidates = populate_candidates(code, b_candidates, n, min_iso);
		
		if (((next_candidates->index + code->index) >= a_len) &&
			(next_b_candidates->index >= min_b_len)) {
			if (code->index == a_len) {
				codeword_list *b_code = new_codeword_list(n, next_b_candidates->index);
				best_len = find_comp(code, b_code, next_b_candidates, n, min_ld,
									 min_b_len, min_len);
				if (best_len >= min_len) {
					longest = best_len;
					min_len = best_len;
					if(find_longest==1)
						min_len++;
				}
				delete_codeword_list(b_code);
			}
			if ((next_candidates->index) && (code->index < a_len)) {
				best_len = find_iso(code, next_candidates, next_b_candidates, n,
									min_ld, min_iso, a_len, min_b_len, min_len);
				if (best_len >= min_len) {
					longest = best_len;
					min_len = best_len;
					if(find_longest==1)
						min_len++;
				}
			}
		}
		delete_codeword_list(next_candidates);
		delete_codeword_list(next_b_candidates);
		code->index--;
	}
	return longest;
}

int find_iso_from_start(uint8_t n, uint8_t min_ld, uint8_t min_iso,
						uint16_t a_len, uint16_t min_b_len,
						codeword_list *candidates) {
	codeword_list *code, *next_candidates, *next_b_candidates;
	int i, longest;
	
	code = new_codeword_list(n, candidates->index);
	for(i=0; i<n; i++)
		code->codewords[i] = 0;
	code->index = 1;
	
	next_candidates = populate_candidates(code, candidates, n, min_ld);
	next_b_candidates = populate_candidates(code, candidates, n, min_iso);
	
	longest = find_iso(code, next_candidates, next_b_candidates, n, min_ld,
					min_iso, a_len, min_b_len, a_len + min_b_len);
	
	delete_codeword_list(next_candidates); 
	delete_codeword_list(next_b_candidates);
	delete_codeword_list(code);
	return longest;
}

void find_best_iso(uint8_t n, uint8_t min_ld, uint8_t min_iso, uint16_t a_len)
{
	uint16_t min_b_len, longest, longest_b;
	codeword_list *candidates;
	min_b_len = 2;
	candidates = create_search_space(n);
	
	do {
		printf("trying a: %d, min b: %d, total: %d\n", a_len,
				min_b_len, a_len + min_b_len);
		longest = find_iso_from_start(n, min_ld, min_iso, a_len, min_b_len,
									  candidates);
		min_b_len = longest - a_len - 3;

	} while (longest > (a_len++ * 2));
	delete_codeword_list(candidates);

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
	
	uint8_t n, min_ld, min_iso;
	if ((n 		 = atoi(argv[optind++])) == 0 ||
		(min_ld  = atoi(argv[optind++])) == 0 ||
		(min_iso = atoi(argv[optind++])) == 0) {
		usage(argv[0]);
		exit(1);
	}
	
	int a_len = 2;
	if (argc > optind) {
		a_len = atoi(argv[optind]);
	}
	
	precompute_ld();
	find_best_iso(n, min_ld, min_iso, a_len);

	return 0;
}
