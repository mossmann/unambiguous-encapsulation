#!/usr/bin/env python

import sys, itertools

ALPHABET_LEN = 7

DEBUG = True
def log(msg):
	if DEBUG:
		print msg

def usage():
	sys.stderr.write("%s: <n> <min_ld> <min_iso>\n" % (sys.argv[0]))

def lee_distance(x, y):
	if len(x) != len(y):
		log("Uneven code lengths: " + str(x) + " " + str(y))
	ld = sum([min((a-b), ALPHABET_LEN-(a-b))
			  for a, b in zip(x, y)])
	return ld

def populate_candidates(code, candidates, min_dist):
	next_candidates = []
	for nc in candidates:
		if lee_distance(nc, code) >= min_dist:
			next_candidates.append(nc)
	return next_candidates

def create_search_space(n):
	# Convert iterator to list because we want to use it twice
	search_space = [x for x in itertools.product(range(ALPHABET_LEN), repeat=n)]
	return search_space

def find_comp(candidates, min_ld, min_len):
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
		
		next_b_candidates = populate_candidates(codeword, b_candidates, min_ld)
		if next_b_candidates and len(next_b_candidates) + len(code) >= min_len:
			cand_stack.append(next_b_candidates)
		else:
			if len(code) >= min_len:
				results.append(code[:])
				min_len = len(code)
			code.pop()
	
	return results, min_len

def find_iso(n, min_ld, min_iso, a_len, min_b_len):
	# Initial setup
	codeword = tuple([0]*n)
	code = [codeword]
	search_space = create_search_space(n)
	search_space.reverse()
	candidates = populate_candidates(code[0], search_space, min_ld)
	b_candidates = populate_candidates(code[0], search_space, min_iso)
	cand_stack = [(candidates, b_candidates)]
	results = []
	second_codewords = {}
	
	while cand_stack:
		candidates, b_candidates = cand_stack[-1]
		if not len(candidates):
			cand_stack.pop()
			code.pop()
			continue
		
		codeword = candidates.pop()
		code.append(codeword)
		
		# De-duplicate second codeword permutations
		if len(code) == 2:
			c = list(codeword)
			c.sort()
			d = tuple(c)
			if d in second_codewords:
				code.pop()
				continue
			else:
				second_codewords[d] = 1
		
		next_b_candidates = populate_candidates(codeword, b_candidates, min_iso)
		if len(code) >= a_len:
			if  len(next_b_candidates) >= min_b_len:
				res, min_b_len = find_comp(next_b_candidates[:], min_ld, min_b_len)
				if res:
					results.append((code[:], res))
			code.pop()
		
		else:
			next_candidates = populate_candidates(codeword, candidates, min_ld)
	
			if len(next_candidates) + len(code) >= a_len and len(next_b_candidates) >= min_b_len:
				cand_stack.append((next_candidates, next_b_candidates))
			else:
				code.pop()
	#print len(second_codewords)
	return results

def find_best_iso(n, min_hd, min_iso):
	a_len = ALPHABET_LEN**(n-1)
	min_b_len = 2
	#valid = []

	while a_len >= min_b_len:
		log("trying a: %d, min b: %d, total: %d" % (a_len, min_b_len, a_len+min_b_len))
		res = find_iso(n, min_hd, min_iso, a_len, min_b_len)
		longest_b = 0
		for (a_code, b_codes) in res:
			for b_code in b_codes:
				print a_len + len(b_code), a_len, len(b_code), a_code, b_code
			#valid.extend([(a_code, b_code) for b_code in b_codes])
			longest_b = reduce(max, map(len, b_codes), longest_b)
		if longest_b >= min_b_len:
			min_b_len = longest_b + 1
		a_len -= 1

	#return valid

if __name__ == '__main__':
	if len(sys.argv) != 4:
		usage()
		sys.exit(1)
	n, min_ld, min_iso = map(int, sys.argv[1:])
	
	find_best_iso(n, min_ld, min_iso)
