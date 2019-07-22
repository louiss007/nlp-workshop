import sys
import os
import codecs
import jieba

# data preprocess for machine translation, step 2, tokenizer.
tools_path = "/home/louiss007/MyWorkShop/model/NLPX/mosesdecoder/scripts"


def english_tokenize(infile, outfile):
    cmd = "perl {tp}/tokenizer/tokenizer.perl -no-escape -l en < {infile} > {outfile}".format(
        tp=tools_path, infile=infile, outfile=outfile)
    ret = os.system(cmd)
    if ret == 0:
        print "english tokenize succeed!"
    else:
        print "english tokenize failed!"


def chinese_tokenize(infile, outfile):
    fo = open(outfile, 'w')
    with codecs.open(infile, 'r', 'utf-8') as fi:
        for line in fi.readlines():
            new_line_list = list()
            for w in line:
                if w.isspace():
                    continue
                new_line_list.append(w)
            new_line = ' '.join(new_line_list).encode('utf-8') + '\n'
            fo.write(new_line)
    fo.close()


def chinese_tokenize_v2(infile, outfile):
    fo = open(outfile, 'w')
    with codecs.open(infile, 'r', 'utf-8') as fi:
        for line in fi.readlines():
            word_list = list(jieba.cut(line, cut_all=False))
            word_list = [word.strip() for word in word_list if not word.isspace()]
            new_line = ' '.join(word_list).encode('utf-8') + '\n'
            fo.write(new_line)
    fo.close()


if __name__ == '__main__':
    if len(sys.argv) != 5:
        print "usage: python %s infile_en infile_zh outfile_en outfile_zh" % __file__
        sys.exit()

    infile_en, infile_zh, outfile_en, outfile_zh = sys.argv[1:]
    # english_tokenize(infile_en, outfile_en)
    chinese_tokenize_v2(infile_zh, outfile_zh)
