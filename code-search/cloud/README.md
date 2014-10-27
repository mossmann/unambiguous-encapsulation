Cloud
=====

This is a collection of hacky scripts that I wrote to manage large scale 
searching for codes.  They mostly rely on a very specific EC2 setup.

Script name and parameters are loaded in to the SQS queue and all of the nodes
pull work from the same queue.  The output is redirected to a file, which is
uploaded to an S3 bucket after the search has completed.

sqs.py is used to read and write data from the SQS queue and upload output to
S3.  It requires AWS keys to be set in your environment in order to work.

spawn.sh is run on the processing nodes using supervisord.  It is a simple
shell script to pull commands from the queue (with sqs.py), run the command and
then upload the results to S2 (again using sqs.py).
