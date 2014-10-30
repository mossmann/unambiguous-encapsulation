#!/usr/bin/env python

# Initial run
for x in range(16, 17):
	for y in range(1, x):
		for z in range(y+1, x):
			print "icblbc %d %d %d" % (x, y, z)

for x in range(16, 17):
	for y in range(1, x):
		print "lcbbc %d %d" % (x, y)

for x in range(20, 21):
	for y in range(1, 41):
		for z in range(y+1, 61):
			print "icnbc %d %d %d" % (x, y, z)
