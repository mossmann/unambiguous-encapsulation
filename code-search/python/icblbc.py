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

def iterative_find_comp(candidates, min_hd, min_len):
	cand_stack = [candidates]
	code = []
	results = []
	while cand_stack:
		b_candidates = cand_stack[-1]
		if not len(b_candidates):
			cand_stack.pop()
			if len(code):
				code.pop()
			continue
		
		codeword = b_candidates.pop()
		code.append(codeword)
		
		next_b_candidates = populate_candidates(codeword, b_candidates, min_hd)
		if next_b_candidates and len(next_b_candidates) + len(code) >= min_len:
			cand_stack.append(next_b_candidates)
		else:
			if len(code) >= min_len:
				results.append(code[:])
				min_len = len(code)
			code.pop()
	
	return results, min_len
	

def iterative_find_iso(n, min_hd, min_iso, a_len, min_b_len):
	# One time setup
	code = [0]
	search_space = range(1, 1<<n)
	search_space.reverse()
	candidates = populate_candidates(code[0], search_space, min_hd)
	b_candidates = populate_candidates(code[0], candidates, min_iso)
	cand_stack = [(candidates, b_candidates)]
	results = []
	
	while cand_stack:
		candidates, b_candidates = cand_stack[-1]
		if not len(candidates):
			cand_stack.pop()
			code.pop()
			continue

		codeword = candidates.pop()
		code.append(codeword)
		
		next_b_candidates = populate_candidates(codeword, b_candidates, min_iso)
		if len(code) >= a_len:
			if  len(next_b_candidates) >= min_b_len:
				res, min_b_len = iterative_find_comp(next_b_candidates[:], min_hd, min_b_len)
				if res:
					results.append((code[:], res))
			code.pop()
		
		else:
			next_candidates = populate_candidates(codeword, candidates, min_hd)
	
			if len(next_candidates) + len(code) >= a_len and len(next_b_candidates) >= min_b_len:
				cand_stack.append((next_candidates, next_b_candidates))
			else:
				code.pop()
	return results

def iterative_find_best_iso(n, min_hd, min_iso):
	a_len = 1<<(n-1)
	min_b_len = 2
	valid = []

	while a_len >= min_b_len:
		log("trying a: %d, min b: %d, total: %d" % (a_len, min_b_len, a_len+min_b_len))
		res = iterative_find_iso(n, min_hd, min_iso, a_len, min_b_len)
		longest_b = 0
		for (a_code, b_codes) in res:
			for b_code in b_codes:
				print a_len + len(b_code), a_len, len(b_code), a_code, b_code
			valid.extend([(a_code, b_code) for b_code in b_codes])
			longest_b = reduce(max, map(len, b_codes), longest_b)
		if longest_b >= min_b_len:
			min_b_len = longest_b + 1
		a_len -= 1

	return valid


if __name__ == '__main__':
	if len(sys.argv) != 4:
		usage()
		sys.exit(1)

	n, min_hd, min_iso = map(int, sys.argv[1:])
	precompute_hd()

	#best = find_best_iso(n, min_hd, min_iso)
	best = iterative_find_best_iso(n, min_hd, min_iso)
	#log(repr(best))
