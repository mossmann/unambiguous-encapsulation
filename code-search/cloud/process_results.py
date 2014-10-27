#!/usr/bin/env python

import redis
import sys
import os.path

icblbc_results = {}
icblbc_l_results = {}
def icblbc(filename, path):
	longest = filename.find('-l') != -1
	args[filename.replace('_', ' ').rstrip(".txt")] = 1 
	with open(os.path.join(path, filename)) as f:
		for line in f:
			if not line.startswith("trying"):
				new_args[filename.replace("_-l", "").replace('_', ' ').rstrip(".txt")] = 1
				break

lcbbc_results = {}
def lcbbc(filename, path):
	if filename.find('-l') != -1:
		# Bad command (error on my part)
		return

icnbc_results = {}
icnbc_l_results = {}
def icnbc(filename, path):
	longest = filename.find('-l') != -1
	args[filename.replace('_', ' ').rstrip(".txt")] = 1 
	with open(os.path.join(path, filename)) as f:
		for line in f:
			if not line.startswith("trying"):
				new_args[filename.replace("_-l", "").replace('_', ' ').rstrip(".txt")] = 1
				break


funcs = {
	"icblbc": icblbc,
	"lcbbc" : lcbbc,
	"icnbc" : icnbc
}

if __name__ == '__main__':
	path = sys.argv[1]
	
	if not os.path.isdir(path):
		print "'%s' is not a directory" % path
		sys.exit(1)
	
	for filename in os.listdir(path):
		classname = filename[:filename.find('_')]
		funcs[classname](filename, path)
