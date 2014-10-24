#!/bin/bash

PATH=$PATH:~/unambiguous-encapsulation/code-search/c:~/unambiguous-encapsulation/code-search/cloud

SQS=sqs.py

while true; do 
	CMD=`$SQS read`
	if [ "$CMD" = "kill" ]; then
		exit 0
	fi
	
	OUTPUT=~/output/`echo $CMD | sed -e "s/ /_/"`.out

	$CMD > $OUTPUT

	$SQS upload $OUTPUT
done
