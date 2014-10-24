#!/bin/bash

CPUS=`fgrep processor /proc/cpuinfo | wc -l`

SQS=./sqs.py

CMD=`$SQS read`

OUTPUT=`echo $CMD | sed -e "s/ /_/"`.out

echo "$CMD > $OUTPUT"
$CMD > $OUTPUT

$SQS upload $OUTPUT
