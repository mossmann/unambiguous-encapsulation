#!/usr/bin/env python

import os.path
import sys
import time

import boto.sqs
from boto.sqs.message import Message
from boto.s3.connection import S3Connection
from boto.s3.key import Key

# AWS_ACCESS_KEY_ID and AWS_SECRET_ACCESS_KEY
# must be set in your environment
def fetch_queue(queuename):
	conn = boto.sqs.connect_to_region("us-west-2")
	queue = conn.get_queue(queuename)
	return queue

def write_message(text, queue):
	m = Message()
	m.set_body(text)
	queue.write(m)

def get_message(queue, block=False):
	msg = None
	while not msg:
		rs = queue.get_messages(num_messages=1)
		if len(rs):
			message = rs[0]
			queue.delete_message(message)
			return message.get_body()
		if not block:
			break
		time.sleep(5)
	return None

def read_from_queue():
	queue = fetch_queue('UE-workqueue')
	msg = get_message(queue, block=True)
	print msg

def write_to_queue(*text):
	queue = fetch_queue('UE-workqueue')
	write_message(' '.join(text), queue)

def write_from_file(filename):
	queue = fetch_queue('UE-workqueue')
	with open(filename) as f:
		for i, line in enumerate(f, 1):
			write_message(line.strip(), queue)
		print "%d messages written from '%s'" % (i, filename)

def upload(filename):
	conn = S3Connection()
	bucket = conn.get_bucket('unambiguous-encapsulation')
	key = Key(bucket)
	key.key = os.path.basename(filename)
	with open(filename) as f:
		key.set_contents_from_string(f.read())
	print "%s Uploades to S3" % filename

def usage(name):
	print '%s <command> [args]' % name
	print '\tcommand should be one of:'
	print '\t\tread'
	print '\t\twrite <message text>'
	print '\t\twrite-file <filename>'
	print '\t\tupload <filename>'

if __name__ == '__main__':
	if len(sys.argv) < 2:
		usage(sys.argv[0])
		sys.exit(1)
	
	funcname = sys.argv[1]
	
	if funcname=="read":
		read_from_queue()
	elif funcname=="write":
		write_to_queue(*sys.argv[2:])
	elif funcname=="write-file":
		write_from_file(sys.argv[2])
	elif funcname=="upload":
		upload(sys.argv[2])
