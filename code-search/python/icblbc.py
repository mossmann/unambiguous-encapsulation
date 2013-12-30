#!/usr/bin/env python

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

n = 0
min_hd = 0
min_iso = 0


'''
Algorithm
  find list of candidates for codes and iso codes assuming start from 0
  for each candidate in the candidate set, check hamming dist to remaining candidates
  eventually producing a list of candidates known to obey min_hd and longer than min_len
  find complementary set
  
  backtrack to find other possible pairs of candidate sets
'''

def populate_candidates(code, candidates, min_dist):
	next_candidates = []
	weights = HD[code]
	for nc in candidates:
		if (weights[nc] >= min_dist):
			next_candidates.append(nc)
	
	return next_candidates


# recursive search for codes complementary to a given code
def find_comp(a_code, candidates, min_b_len, min_len):
	longest = 0
	cand_list = [(a_code, [], candidates)]
	
	while cand_list:
		(a_code, b_code, candidates) = cand_list.pop()
		
		while candidates:
			c = candidates.pop()
			b_code.append(c)
			next_candidates = populate_candidates(c, candidates, min_hd)
	
			# only look for b_code at least as long as the longest we have found
			if (len(next_candidates) + len(b_code)) >= min_b_len:
				if (len(next_candidates) + len(b_code) + len(a_code)) >= min_len:
					if len(next_candidates):
						cand_list.append((a_code, b_code, next_candidates))
						break
					else:
						print "%d %d %d %s %s" % (len(a_code) + len(b_code), len(a_code), len(b_code), a_code, b_code)
						if (len(a_code) + len(b_code)) >= min_len:
							min_len = longest = len(a_code) + len(b_code)
	
			b_code.pop()

	return longest

# recursive search for pairs of complementary codes 
def find_iso(code, candidates, b_candidates, a_len, min_b_len, min_len):
	longest = 0
	cand_list = [(code, candidates, b_candidates)]
	
	while cand_list:
		(code, candidates, b_candidates) = cand_list.pop()
		
		while candidates:
			c = candidates.pop()
			code.append(c)
			next_candidates = populate_candidates(c, candidates, min_hd)
			next_b_candidates = populate_candidates(c, b_candidates, min_iso)
		
			if (len(next_candidates) + len(code)) >= a_len \
				and len(next_b_candidates) >= min_b_len:
				if len(code) == a_len:
					best_len = find_comp(code, next_b_candidates,
										min_b_len, min_len)
					if best_len >= min_len:
						min_len = longest = best_len
		
				if len(next_candidates) and (len(code) < a_len):
					cand_list.append((code[0:-1], candidates, b_candidates))
					cand_list.append((code, next_candidates,next_b_candidates))
					break
					#best_len = find_iso(a_len, min_b_len, min_len)
					#if best_len >= min_len:
					#	min_len = longest = best_len
			code.pop()

	return longest


def find_best_iso():
	min_b_len = 2
	a_len = 1<<(n-1)

	while a_len >= min_b_len:
		print "trying a: %d, min b: %d, total: %d" % (a_len, min_b_len,
													  a_len + min_b_len)
		start = 0
		code = [start]
		candidates = range(1<<n)
		next_candidates = populate_candidates(start, candidates, min_hd)
		next_b_candidates = populate_candidates(start, candidates, min_iso)
		longest = find_iso(code, next_candidates,next_b_candidates, a_len, min_b_len, a_len + min_b_len)

		if longest >= (a_len + min_b_len):
			min_b_len = longest - a_len + 1
		a_len -= 1


if __name__ == '__main__':
	precompute_hd()
	n = 5
	min_hd = 2
	min_iso = 3
	find_best_iso()
