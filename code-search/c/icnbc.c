#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void usage(char *argv0) {
	fprintf(stderr, "%s: <n> <min_hd> <min_iso>\n", argv0);
}

int main(int argc, char** argv)
{
	if (argc < 4) {
		usage(argv[0]);
		exit(1);
	}

	int n, min_hd, min_iso;
	if ((n 		 = atoi(argv[1])) == 0 ||
		(min_hd  = atoi(argv[2])) == 0 ||
		(min_iso = atoi(argv[3])) == 0) {
		usage(argv[0]);
		exit(1);
	}

	//max_cand = 1 << n;
	//cand_len = (max_cand+7) / 8;
	//
	//precompute_hd(min_hd);
	//find_code();
	//
	//free(HD);
	return 0;
}
