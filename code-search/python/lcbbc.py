#!/usr/bin/env python

import sys
import math

DEBUG = True
def log(msg):
	if DEBUG:
		print msg

def usage():
	sys.stderr.write("%s: <n> <min_hd>\n" % (sys.argv[0]))


MAX_N = 8
MAX_CAND = (1 << MAX_N)

HAMMING_WEIGHT = [0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8]
HD = []

# fill big lookup table of hamming distances
def precompute_hd(min_hd):
	for i in range(MAX_CAND):
		HD.append([])
		for j in range(MAX_CAND):
			if HAMMING_WEIGHT[i ^ j] >= min_hd:
				HD[i].append(True)
			else:
				HD[i].append(False)


'''
Algorithm
  find list of candidates for codes assuming start from 0 for each candidate in
  the candidate set, check hamming dist to remaining candidates eventually
  producing a code known to obey min_hd.
  
  backtrack to find other possible codes with greater length
'''

def populate_candidates(code, candidates):
	return filter(lambda x: HD[code][x], candidates)

def iterative_find_code(n):
	# One time setup
	code = [0]
	longest_code = 1
	search_space = range((1<<n)-1, 0, -1)
	candidates = populate_candidates(code[0], search_space)
	cand_stack = [candidates]
	results = []
	
	while cand_stack:
		candidates = cand_stack[-1]
		if not len(candidates):
			cand_stack.pop()
			code.pop()
			continue

		codeword = candidates.pop()
		code.append(codeword)
		next_candidates = populate_candidates(codeword, candidates)
	
		if next_candidates and len(next_candidates) + len(code) > longest_code:
			cand_stack.append(next_candidates)
		else:
			if len(code) > longest_code:
				results.append(code[:])
				log("%d, %f, " % (len(code), math.log(len(code), 2)))
				log(code)
				longest_code = len(code)
			code.pop()
	return results


if __name__ == '__main__':
	if len(sys.argv) != 3:
		usage()
		sys.exit(1)

	n, min_hd = map(int, sys.argv[1:])
	precompute_hd(min_hd)

	results = iterative_find_code(n)
	#for code in results:
	#	log(code)
