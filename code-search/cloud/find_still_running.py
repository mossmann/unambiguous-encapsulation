#!/usr/bin/env python

import sys
import os.path

completed = {}

for filename in os.listdir(sys.argv[1]):
    completed[filename] = 1

for filename in sys.argv[2:]:
    with open(filename) as f:
        for task in f:
            output = task.strip().replace(' ', '_') + ".txt"
            if output not in completed:
                print task.strip()
