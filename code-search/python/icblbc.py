#!/usr/bin/env python

import sys

DEBUG = True
def log(msg):
	if DEBUG:
		print msg

def usage():
	sys.stderr.write("%s: <n> <min_hd> <min_iso>\n" % (sys.argv[0]))


MAX_N = 8
MAX_CAND = (1 << MAX_N)

HAMMING_WEIGHT = [0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8]
HD = []

# fill big lookup table of hamming distances
def precompute_hd():
	for i in range(MAX_CAND):
		HD.append([])
		for j in range(MAX_CAND):
			HD[i].append(HAMMING_WEIGHT[i ^ j])


'''
Algorithm
  find list of candidates for codes and iso codes assuming start from 0
  for each candidate in the candidate set, check hamming dist to remaining
  candidates eventually producing a list of candidates known to obey min_hd and
  a_depth long. Next find complementary set with hamming distance min_iso from
  every element in original set, this set should be b_depth long.
  
  backtrack to find other possible pairs of candidate sets
'''

def populate_candidates(code, candidates, min_dist):
	next_candidates = []
	weights = HD[code]
	for nc in candidates:
		if (weights[nc] >= min_dist):
			next_candidates.append(nc)

	return next_candidates


# recursive search for pairs of complementary codes
def find_iso(code, candidates, b_candidates, a_depth, b_depth):
	a_depth -= 1
	results = []
	for i, c in enumerate(candidates, 1):
		code.append(c)
		next_b_candidates = populate_candidates(c, b_candidates, min_iso)

		if b_depth and a_depth == 0:
			b_code = []
			res = find_iso(b_code, next_b_candidates, [], b_depth, 0)
			if res:
				results.append((code[:], res))
		if a_depth == b_depth == 0:
			results.extend(code[:])
		else:
			next_candidates = populate_candidates(c, candidates[i:-1], min_hd)

			if next_candidates >= a_depth and len(next_b_candidates) >= b_depth:
				res = find_iso(code, next_candidates,next_b_candidates, a_depth, b_depth)
				if res:
					results.extend(res)

		code.pop()
	return results


def find_best_iso(n, min_hd, min_iso):
	a_depth = 1<<(n-1)
	b_depth = 2
	start = 0
	candidates = range(1<<n)
	log(repr(candidates))

	valid = []

	while a_depth > b_depth and a_depth + b_depth > n:
		log("trying a: %d, b: %d" % (a_depth, b_depth))
		a_code = [start]
		a_depth -= 1
		next_candidates = populate_candidates(start, candidates, min_hd)
		next_b_candidates = populate_candidates(start, candidates, min_iso)
		res = find_iso(a_code, next_candidates, next_b_candidates,
					   a_depth, b_depth)
		for (i, j) in res:
			valid.append((i, j))
		if res and a_depth > b_depth:
			b_depth += 1
		else:
			a_depth -= 1
	return valid

if __name__ == '__main__':
	if len(sys.argv) != 4:
		usage()
		sys.exit(1)

	n, min_hd, min_iso = map(int, sys.argv[1:])
	precompute_hd()

	best = find_best_iso(n, min_hd, min_iso)
	log(repr(best))
