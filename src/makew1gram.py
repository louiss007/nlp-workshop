#!/usr/bin/python
import jieba
import sys

def makew1gram(infile, outfile) :
    fi = open(infile, 'r')
    fo = open(outfile, 'w')
    for line in fi.readlines() :
        words = jieba.cut(line.strip(), cut_all = False)
        words = [word.strip() for word in words]
        for w in words :
            fo.write(w.encode('utf8') + '\n')
    fi.close()
    fo.close()
infile, outfile = sys.argv[1:]
makew1gram(infile, outfile)
