import sys
import json
import codecs

# data preprocessing for machine translation, step 1, obtain bilingual data


def read_data(in_file, out_file_en, out_file_zh):
    fi = codecs.open(in_file, 'r', 'utf-8')
    fo_en = open(out_file_en, 'w')
    fo_zh = open(out_file_zh, 'w')
    for line in fi.readlines():
        json_object = json.loads(line.strip())
        sentence_en = json_object['english']
        sentence_zh = json_object['chinese']
        fo_en.write(sentence_en.encode('utf-8') + '\n')
        fo_zh.write(sentence_zh.encode('utf-8') + '\n')

    fi.close()
    fo_en.close()
    fo_zh.close()


if __name__ == '__main__':
    if len(sys.argv) != 4:
        print "usage: python %s infile outfile_en outfile_zh"
        sys.exit()

    infile, outfile_en, outfile_zh = sys.argv[1:]
    read_data(infile, outfile_en, outfile_zh)
