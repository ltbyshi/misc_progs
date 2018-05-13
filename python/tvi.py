#! /usr/bin/env python
from __future__ import print_function
from six.moves import cStringIO as StringIO
import sys
import argparse
import os
import errno

def main():
	parser = argparse.ArgumentParser(description='Print tables pretty')
	parser.add_argument('infile', type=str, nargs='?',
					 help='input file, default is stdin')
	parser.add_argument('-d', dest='delimiter', type=str,
					 required=False,
					 help='delimiter of fields of input. Default is white space.')
	parser.add_argument('-j', dest='justify', type=str,
					 required=False, default='left', 
					 choices=['left', 'right', 'center'],
					 help='justification, either left, right or center. Default is left')
	parser.add_argument('-s', dest='separator', type=str,
					 required=False, default=' ',
					 help='separator of fields in output')
	args = parser.parse_args()
	
	table = []
	maxwidth = []
	
	# default is to read from stdin
	fin = sys.stdin
	if args.infile:
		try:
			fin = open(args.infile, 'rt')
		except IOError as e:
			print('Error: %s: %s\n'%(e.strerror, args.infile), file=sys.stderr)
			sys.exit(e.errno)

	try:
		for line in fin:
			fields = None
			# split line by delimiter
			if args.delimiter:
				fields = line.strip().split(args.delimiter)
			else:
				fields = line.strip().split()
			for i in xrange(len(fields)):
				width = len(fields[i])
				if (i+1) > len(maxwidth):
					maxwidth.append(width)
				else:
					if width > maxwidth[i]:
						maxwidth[i] = width
			table.append(fields)
		fin.close()
	
		for fields in table:
			line = StringIO()
			for i in xrange(len(fields)):
				# format field with different justification
				nSpace = maxwidth[i] - len(fields[i])
				if args.justify == 'left':
					line.write(fields[i])
					for j in xrange(nSpace):
						line.write(' ')
				elif args.justify == 'right':
					for j in xrange(nSpace):
						line.write(' ')
					line.write(fields[i])
				elif args.justify == 'center':
					for j in xrange(nSpace/2):
						line.write(' ')
					line.write(fields[i])
					for j in xrange(nSpace - nSpace/2):
						line.write(' ')
				
				line.write(args.separator)
			print(line.getvalue())
			line.close()
	except IOError as e:
		if e.errno == errno.EPIPE:
			sys.exit(-e.errno)
		else:
			raise e
	except KeyboardInterrupt:
		pass
		
if __name__ == '__main__':
	main()
