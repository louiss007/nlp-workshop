import codecs
import sys

# data preprocess for machine translation, step 4, convert word to id.
# RAW_DATA = "../data/text_data/ptb/ptb.valid.txt"
# VOCAB = "../data/text_data/ptb/ptb.vocab"
# OUTPUT_DATA = "../data/text_data/ptb/ptb.valid"


def get_word2id(VOCAB):
    with codecs.open(VOCAB, "r", "utf-8") as fv:
        vocab = [w.strip() for w in fv.readlines()]
    word2id = {k: v for (k, v) in zip(vocab, range(len(vocab)))}
    return word2id


def get_id(word):
    return word2id[word] if word in word2id else word2id['<unk>']


def convert_word_to_id(RAW_DATA, OUTPUT_DATA):
    fin = codecs.open(RAW_DATA, "r", "utf-8")
    fout = codecs.open(OUTPUT_DATA, "w", 'utf-8')
    for line in fin:
        words = line.strip().split() + ["<eos>"]
        out_line = ' '.join([str(get_id(w)) for w in words]) + '\n'
        fout.write(out_line)
    fin.close()
    fout.close()


if __name__ == '__main__':
    if len(sys.argv) != 4:
        print "usage: python %s infile outfile vocab_file" % __file__
        sys.exit()

    infile, outfile, vocab_file = sys.argv[1:]
    word2id = get_word2id(vocab_file)
    convert_word_to_id(infile, outfile)