#!/usr/bin/env python

from math import factorial

# q = number of letters in alphabet (e.g. 2 for binary codes)
# n = length of codeword
# d = minimum hamming distance

def hammingbound(q, n, d):
    t = (d - 1) / 2
    print t
    sum = 0
    for k in range(t + 1):
        sum += (factorial(n) / (factorial(k) * factorial(n - k))) * (q - 1)**k
        print sum
    return (1.0 * q**n) / sum

import sys

if len(sys.argv) != 4:
	sys.exit(1)

q, n, d = map(int, sys.argv[1:])
print hammingbound(q, n, d)
