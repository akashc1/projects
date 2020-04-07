"""
Akash Chaurasia
akashc@jhu.edu
"""

from tag_counter import TagCounter

"""
Parses training data file
Creates TagCounter object and adds all training instances
to object
"""
def parse_train_data(train_file, lamb=None):

    f = open(train_file)

    first_line = f.readline().strip().split('/')

    prev_tag = first_line[1]
    prev_word = first_line[0]
    seq = [prev_word]
    tags = [prev_tag]

    tc = TagCounter(lamb)

    for line in f:

        split = line.strip().split('/')
        word, tag = split[0], split[1]

        tc.add(word, tag, prev_tag)
        seq.append(word)
        tags.append(tag)

        prev_tag = tag
        prev_word = word

    f.close()

    return tc, seq, tags

"""
Parses test data file an dreturns sequence of words
and sequence of true tags (only used to calculate accuracy)
"""
def parse_test_data(test_file):

    f = open(test_file)

    seq = []
    tags = []

    for line in f:

        split = line.strip().split('/')
        word, tag = split[0], split[1]

        seq.append(word)
        tags.append(tag)

    f.close()

    return seq, tags

"""
Gets raw data from a file, returns the sequence of words
"""
def parse_raw_data(raw_file):

    f = open(raw_file)

    seq = []

    for line in f:
        seq.append(line.strip())

    f.close()
    return seq

"""
Writes the output file indicating the calculated tagging
of words using any algorithm
"""
def write_output(seq, best_tags):

    f = open("test-output", "w+")

    for i, word in enumerate(seq):
        f.write(f"{word}/{best_tags[i]}\n")

    f.close()
