#!/usr/bin/python
#coding: utf8

import os, sys

for line in sys.stdin:
    arr=line.split()
    if len(arr)!=3: continue
    seq=arr[0][:-1]
    score=int(arr[1])

    #if score*score>4: continue

    n=len(seq)
    for i in xrange(0, n, 3):
        a=seq[i]
        b=seq[i+1:i+3]
        if a=='+':
            print b.upper(), score, 
        elif a=='-':
            print b, -score, 
    print


