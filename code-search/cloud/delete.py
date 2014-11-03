#!/usr/bin/env python

from boto.s3.connection import S3Connection

conn = S3Connection()
bucket = conn.get_bucket('unambiguous-encapsulation')

count = 0
for version in bucket.list_versions():
	bucket.delete_key(version.name, version_id = version.version_id)
	count += 1
	if count%100 == 0:
		print count

bucket.delete()
