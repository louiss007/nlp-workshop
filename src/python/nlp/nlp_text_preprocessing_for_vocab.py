import sys
import codecs
import collections
from operator import itemgetter


# data preprocess for machine translation, step 3, build bilingual vocab.
# RAW_DATA = "../data/text_data/ptb/ptb.train.txt"
# VOCAB_OUTPUT = "../data/text_data/ptb/ptb.vocab"


counter = collections.Counter()


def build_vocab(RAW_DATA, VOCAB_OUTPUT, lang):
    with codecs.open(RAW_DATA, "r", "utf-8") as f:
        for line in f:
            for word in line.strip().split():
                counter[word] += 1
    sorted_word_to_cnt = sorted(counter.items(),
                                key=itemgetter(1),
                                reverse=True)

    cnt = 0
    sorted_words_freq = []
    for item in sorted_word_to_cnt:
        if item[1] < 5:
            break
        sorted_words_freq.append(item)
        cnt += 1
    print "word num: %d" % cnt
    sorted_words = [x[0] for x in sorted_words_freq]
    # sorted_words = ["<eos>"] + sorted_words

    sorted_words = ["<unk>", "<sos>", "<eos>"] + sorted_words
    if lang == 'en':
        if len(sorted_words) > 150000:
            sorted_words = sorted_words[:150000]
    else:
        if len(sorted_words) > 180000:
            sorted_words = sorted_words[:180000]

    with codecs.open(VOCAB_OUTPUT, 'w', 'utf-8') as fo:
        for word in sorted_words:
            fo.write(word + '\n')


if __name__ == '__main__':
    if len(sys.argv) != 4:
        print "usage: python %s infile outfile lang" % __file__
        sys.exit()

    infile, outfile, lang = sys.argv[1:]
    build_vocab(infile, outfile, lang)