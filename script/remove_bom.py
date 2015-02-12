#!/usr/bin/python
#coding:utf8

import os, sys

s='\xef\xbb\xbf'


if __name__=='__main__':
    if len(sys.argv)!=2:
        print 'Usage: %s FILE\nRemove Head BOM (3 bytes) from FILE' % sys.argv[0]
        sys.exit(1)
    fp=sys.argv[1]
    f=open(fp)
    t=f.read()
    f.close()
    if t.startswith(s):
        t=t[3:]
        f=open(fp, 'w')
        f.write(t)
        f.close()
        print >>sys.stderr,'BOM removed from', fp
    else:
        print >>sys.stderr,'nothing to do with', fp

