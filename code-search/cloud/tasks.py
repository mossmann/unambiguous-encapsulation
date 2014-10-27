#!/usr/bin/env python

# Initial run
for x in range(2, 16):
	for y in range(1, x):
		for z in range(y+1, x):
			print "icblbc -l %d %d %d" % (x, y, z)

for x in range(2, 16):
	for y in range(1, x):
		print "lcbbc %d %d" % (x, y)

for x in range(2, 20):
	for y in range(1, 40):
		for z in range(y+1, 60):
			print "icnbc -l %d %d %d" % (x, y, z)
