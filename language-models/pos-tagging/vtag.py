"""
Akash Chaurasia
akashc@jhu.edu
"""

import numpy as np
import argparse
import fileio as io
from tag_counter import TagCounter
from scipy.special import logsumexp

"""
Parses arguments
"""
def parse_args():

    parser = argparse.ArgumentParser()

    parser.add_argument("train_data", help="Training data file")

    parser.add_argument("test_data", help="Test data file")

    args = parser.parse_args()

    return args

"""
Calculates accuracy of different types as indicated by assignment
Prints perplexity if requested by last argument
Returns the different types of accuracy
"""
def calc_accuracy(true_seq, calc_seq, test_seq, tag_counts, perp=False):

    known_correct = 0
    known_count = 0

    total_perp = 0
    total_count = 0

    new_correct = 0
    new_count = 0

    # iterate through given sequence
    for i in range(1, len(true_seq)):

        # add to total perplexity
        total_perp += tag_counts.get_wt_prob(test_seq[i], true_seq[i])
        total_perp += tag_counts.get_tt_prob(true_seq[i - 1], true_seq[i])
        total_count += 1

        # if seen in training, add to known count/accuracy
        if tag_counts.known_word(test_seq[i]):
            if true_seq[i] == '###':
                continue
            known_count += 1
            if true_seq[i] == calc_seq[i]:
                known_correct += 1
        # new word, add to new count/accuracy
        else:
            if true_seq[i] == calc_seq[i]:
                if true_seq[i] == '###':
                    continue
                new_correct += 1
            new_count += 1

    # calculate accuracies
    known_acc = known_correct / known_count
    new_acc = 0
    if new_count > 0:
        new_acc = new_correct / new_count

    # calculate and report average perplexity if requested
    total_perp *= -1 / total_count
    total_perp = np.exp(total_perp)

    if perp:
        print(f"Model perplexity per tagged test word: {total_perp:.3f}")

    total_acc = (new_correct + known_correct) / (known_count + new_count)

    return known_acc, new_acc, total_acc

"""
Performs viterbi decoding given counts and test sequence
Returns the best tags
"""
def viterbi(seq, tag_counts):

    # initialize variables to be used
    all_tags = list(tag_counts.t_counts.keys())
    num_states = len(seq)
    tag_inds = {}

    # use 2d arrays to represent partial probabilities and backpointers
    partial_probs = np.zeros((len(all_tags), num_states)) + np.NINF
    backpointers = np.zeros((len(all_tags), num_states), dtype=int)

    # initialize tag index dictionary and partial probabilities using tag probs
    for i, tag in enumerate(all_tags):
        tag_inds[tag] = i
        partial_probs[i, 0] = (tag_counts.get_tag_prob(tag)
                                + tag_counts.get_wt_prob(seq[0], tag))

    # intialize first states backpointers and first tag prob
    backpointers[:, 0] = tag_inds['###']
    partial_probs[tag_inds['###'], 0] = 0.0

    for i in range(1, num_states): 
        # get current and previous tags
        word = seq[i]
        prev_word = seq[i - 1]
        word_tags = tag_counts.get_word_tags(word)
        prev_word_tags = tag_counts.get_word_tags(prev_word)

        for tag in word_tags:
            # emission probability (p(w | t))
            wt_prob = tag_counts.get_wt_prob(word, tag)

            tag_ind = tag_inds[tag]

            for prev_tag in prev_word_tags:

                prevtag_ind = tag_inds[prev_tag]
                # arc probability
                p = (tag_counts.get_tt_prob(prev_tag, tag) + wt_prob)
                # prob of best sequence ending in this arc
                u = (partial_probs[prevtag_ind, i - 1] + p)

                # update partial probs and backpointer if higher prob
                if u > partial_probs[tag_ind, i]:
                    partial_probs[tag_ind, i] = u
                    backpointers[tag_ind, i] = prevtag_ind

    # start at end
    cur_tag = '###'
    result = [cur_tag]

    # go through backpointers to get forward sequence
    for i in range((num_states - 1), 0, -1):
        cur_tag = backpointers[tag_inds[cur_tag], i]
        result.insert(0, all_tags[cur_tag])
        cur_tag = all_tags[cur_tag]

    return result

"""
Performs forward-backward algorithm as indicated in assignment
Uses the given untagged sequence and training counts
Returns the best tags using posterior decoding
"""
def forward_backward(seq, tag_counts):

    # gets all tags
    all_tags = list(tag_counts.t_counts.keys())
    num_states = len(seq)
    tag_inds = {}

    # initialize all forward probabilities as 0 (in log space)
    f_probs = np.zeros((len(all_tags), num_states)) + np.NINF

    # initializing initial state probabilities as prob of tag and that wt pair
    for i, tag in enumerate(all_tags):
        tag_inds[tag] = i
        f_probs[i, 0] = (tag_counts.get_tag_prob(tag)
                                + tag_counts.get_wt_prob(seq[0], tag))

    # initialize start state of ### as p = 1
    f_probs[tag_inds['###'], 0] = 0.0

    # iterate through states
    for i in range(1, num_states):
        word = seq[i]
        prev_word = seq[i - 1]
        word_tags = tag_counts.get_word_tags(word)
        prev_word_tags = tag_counts.get_word_tags(prev_word)

        # iterate through tags possible for this word
        for tag in word_tags:

            # get emission prob
            wt_prob = tag_counts.get_wt_prob(word, tag)
            tag_ind = tag_inds[tag]

            # iterate through tags possible for previous word
            for prev_tag in prev_word_tags:

                prevtag_ind = tag_inds[prev_tag]

                # sum probabilities over possible previous tags
                p = tag_counts.get_tt_prob(prev_tag, tag) + wt_prob
                to_sum = [p + f_probs[prevtag_ind, i - 1], f_probs[tag_ind, i]]
                f_probs[tag_ind, i] = logsumexp(to_sum)

    Z = f_probs[tag_inds['###'], -1]

    # initialize backward probabilities as 0 except for ### at final state
    b_probs = np.zeros((len(all_tags), num_states)) + np.NINF
    b_probs[tag_inds['###'], -1] = 0.0

    # iterate backwards over sequence
    for i in range(num_states - 1, 0, -1):
        word = seq[i]
        prev_word = seq[i - 1]
        word_tags = tag_counts.get_word_tags(word)
        prev_word_tags = tag_counts.get_word_tags(prev_word)

        # iterate through possible tags for this word
        for tag in word_tags:

            wt_prob = tag_counts.get_wt_prob(word, tag)
            tag_ind = tag_inds[tag]

            # iterate through possible previous tags
            for prev_tag in prev_word_tags:
                prevtag_ind = tag_inds[prev_tag]

                # sum over all paths leading to previous tag
                p = tag_counts.get_tt_prob(prev_tag, tag) + wt_prob
                to_sum = [b_probs[prevtag_ind, i - 1], p + b_probs[tag_ind, i]]
                b_probs[prevtag_ind, i - 1] = logsumexp(to_sum)

    # perform posterior decoding to find best tags for given sequence
    result = []

    for i in range(len(seq)):
        result.append(all_tags[np.argmax(f_probs[:, i] + b_probs[:, i])])

    return result


def main():

    # parse arguments
    args = parse_args()

    # parse train/test data
    tag_counts, seq, true_tags = io.parse_train_data(args.train_data, 5)
    test_seq, test_tags = io.parse_test_data(args.test_data)

    # use viterbi decoding with calcualted counts, report accuracy & perplexity
    best_seq = viterbi(test_seq, tag_counts)
    known_acc, new_acc, total_acc = calc_accuracy(test_tags, best_seq, test_seq, tag_counts, True)

    print(f"Tagging accuracy (Viterbi decoding): {100 * total_acc:.2f}%", end="")
    print(f"\t(known: {100 * known_acc:.2f}%\tnovel: {100 * new_acc:.2f}%)")

    # use forward-backward algo and posterior decoding, report accuracy & perplexity
    best_seq = forward_backward(test_seq, tag_counts)
    known_acc, new_acc, total_acc = calc_accuracy(test_tags, best_seq, test_seq, tag_counts)

    print(f"Tagging accuracy (posterior decoding): {100 * total_acc:.2f}%", end="")
    print(f"\t(known: {100 * known_acc:.2f}%\tnovel: {100 * new_acc:.2f}%)")

    # write output file
    io.write_output(test_seq, best_seq)

if __name__ == "__main__":
    main()
