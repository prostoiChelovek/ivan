import re
import sys


def removeDuplicates(seq):
    seen = set()
    seen_add = seen.add
    return [x for x in seq if not (x in seen or seen_add(x))]


pattern = r"(\b{word}\b)|({word}\()"

words_file_path = sys.argv[1]
source_file_path = sys.argv[2]
dst_file_path = sys.argv[3]

words_file = open(words_file_path, "r")
source_file = open(source_file_path, "r")
source_file_enum = enumerate(source_file)
dst_file = open(dst_file_path, "w")

regexes = []
words = []

results = []

for i, line in enumerate(words_file):
    word = line.rstrip()
    words.append(word)
    word = word.replace("'", "").replace("`", "").replace("-", "")
    regex = pattern.format(word=word)
    regexes.append(regex)
    print(regex)

for j, src_line in source_file_enum:
    i = 0
    line = src_line.replace("'", "").replace("-", "").replace("`", "")
    for regex in regexes:
        if re.match(regex, line, re.IGNORECASE):
            results.append(src_line.replace("'", "`"))
            print('Word "%s" found on line %s - %s' % (words[i], j + 1, src_line))
        i += 1

results = removeDuplicates(results)

for i, result in enumerate(results):
    spl = result.split()
    spl[0] = spl[0].lower()
    results[i] = " ".join(spl) + "\n"

dst_file.writelines(results)
