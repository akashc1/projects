"""
Akash Chaurasia
akashc@jhu.edu
"""

import numpy as np
import argparse
import fileio as io
from tag_counter import TagCounter, WT_Pair, TT_Pair
from scipy.special import logsumexp
import copy

# parse arguments
def parse_args():

    parser = argparse.ArgumentParser()

    parser.add_argument("train_data", help="Training data file")

    parser.add_argument("test_data", help="Test data file")

    parser.add_argument("raw_data", help="Unlabeled data for EM algorithm")

    args = parser.parse_args()

    return args

'''
Calculates accuracy as needed for given types
Reports perplexity if asked
'''
def calc_accuracy(true_seq, calc_seq, test_seq, tag_counts, perp=False):

    known_correct = 0
    known_count = 0

    seen_correct = 0
    seen_count = 0

    total_perp = 0
    total_count = 0

    new_correct = 0
    new_count = 0

    # iterate over given sequence
    for i in range(1, len(true_seq)):

        # calculate perplexity
        total_perp += tag_counts.get_wt_prob(test_seq[i], true_seq[i])
        total_perp += tag_counts.get_tt_prob(true_seq[i - 1], true_seq[i])
        total_count += 1

        # if known word, and to known count/accuracy
        if tag_counts.known_word(test_seq[i]):
            if true_seq[i] == '###':
                continue
            known_count += 1
            if true_seq[i] == calc_seq[i]:
                known_correct += 1
        # if seen word, add to seen count/accuracy
        elif tag_counts.seen_word(test_seq[i]):
            if true_seq[i] == '###':
                continue
            seen_count += 1
            if true_seq[i] == calc_seq[i]:
                seen_correct += 1
        # not known or seen, add to new count/accuracy
        else:
            if true_seq[i] == calc_seq[i]:
                if true_seq[i] == '###':
                    continue
                new_correct += 1
            new_count += 1

    # calculate accuracies and perplexity
    known_acc = known_correct / known_count
    new_acc = 0
    seen_acc = 0
    if new_count > 0:
        new_acc = new_correct / new_count

    if seen_count > 0:
        seen_acc = seen_correct / seen_count

    total_perp *= -1 / total_count
    total_perp = np.exp(total_perp)

    if perp:
        print(f"Model perplexity per tagged test word: {total_perp:.3f}")

    total_acc = ((new_correct + known_correct + seen_correct) /
            (known_count + new_count + seen_count))

    return known_acc, new_acc, total_acc, seen_acc

'''
Performs viterbi decoding over the given sequence using the given counts

Returns the best tags using decoding
'''
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

'''
Performs forward-backward algorithm
'''
def forward_backward(seq, tag_counts, all_tags, tag_inds,
                     all_words):

    num_states = len(seq)

    # initialize forward probabilities
    f_probs = np.zeros((len(all_tags), num_states)) + np.NINF
    f_probs[tag_inds['###'], 0] = 0.0

    # iterate forward across states
    for i in range(1, num_states):
        word = seq[i]
        prev_word = seq[i - 1]
        word_tags = tag_counts.get_word_tags(word)
        prev_word_tags = tag_counts.get_word_tags(prev_word)

        # go through all possible tags for this word
        for tag in word_tags:

            # get emission prob
            wt_prob = tag_counts.get_wt_prob(word, tag)
            tag_ind = tag_inds[tag]

            # first time has a different starting prob to initialize alpha correctly
            if i == 1:
                f_probs[tag_ind, i] = wt_prob + tag_counts.get_tt_prob('###', tag)
                continue

            # iterate through all tags for previous word
            for prev_tag in prev_word_tags:

                # sum probabilities over previous tags
                prevtag_ind = tag_inds[prev_tag]
                p = tag_counts.get_tt_prob(prev_tag, tag) + wt_prob
                p += f_probs[prevtag_ind, i - 1]

                to_sum = [p, f_probs[tag_ind, i]]
                f_probs[tag_ind, i] = logsumexp(to_sum)

    # initialize backward probs
    b_probs = np.zeros((len(all_tags), num_states)) + np.NINF
    b_probs[tag_inds['###'], -1] = 0.0

    # iterate backwards through time stamps
    for i in range(num_states - 1, 0, -1):
        word = seq[i]
        prev_word = seq[i - 1]
        word_tags = tag_counts.get_word_tags(word)
        prev_word_tags = tag_counts.get_word_tags(prev_word)

        # iterate through possible tags for this word
        for tag in word_tags:

            # get emission probability
            wt_prob = tag_counts.get_wt_prob(word, tag)
            tag_ind = tag_inds[tag]

            # different beta for second-to-last step
            if i == num_states - 1:
                for prev_tag in prev_word_tags:
                    prevtag_ind = tag_inds[prev_tag]
                    b_probs[prevtag_ind, i - 1] = tag_counts.get_tt_prob(prev_tag, '###')
                continue

            # iterate through possible tags for previous word
            for prev_tag in prev_word_tags:
                prevtag_ind = tag_inds[prev_tag]

                # sum over probs for previous tags
                p = tag_counts.get_tt_prob(prev_tag, tag) + wt_prob
                to_sum = [b_probs[prevtag_ind, i - 1], p + b_probs[tag_ind, i]]
                b_probs[prevtag_ind, i - 1] = logsumexp(to_sum)

    return f_probs, b_probs

'''
Does the tedious task of EM for 10 iterations using raw data
Returns the posterior decoding after 10 iterations
'''
def em_tagging(test_seq, test_tags, init_counts, raw_seq):

    # get standard tags and indices for uniform use
    all_tags, all_words, tag_inds = init_counts.get_count_tables()

    # initialize cur and new counts with a deep copy
    cur_counts = copy.deepcopy(init_counts)
    new_counts = copy.deepcopy(init_counts)

    # calculate initial accuracy and perplexity with viterbi
    calc_seq = viterbi(test_seq, init_counts)
    known_acc, new_acc, total_acc, seen_acc = calc_accuracy(test_tags, calc_seq, test_seq, init_counts, True)

    print(f"Tagging accuracy (Viterbi decoding): {100 * total_acc:.2f}%", end="")
    print(f"\t(known: {100 * known_acc:.2f}%  seen: {100 * seen_acc:.2f}%", end="")
    print(f"  novel: {100 * new_acc:.2f}%)")

    # perform 10 (technically 11) iterations of EM
    for n in range(11):

        # use forward-backward to get alphas and betas
        f_probs, b_probs = forward_backward(raw_seq, cur_counts,
                                            all_tags, tag_inds, all_words)

        # calculate Z, which is constant across all steps
        tag_probs = f_probs[:, 1] + b_probs[:, 1]

        Z = np.NINF

        for p in tag_probs:
            Z = logsumexp([Z, p])

        # get the probability of any tag at a given time
        tag_probs = f_probs + b_probs - Z

        # M step, get new counts and add to original by iterating over raw
        # to get expected counts
        for i in range(1, len(raw_seq)):

            word = raw_seq[i]
            prev_word = raw_seq[i - 1]

            cur_tags = cur_counts.get_word_tags(word)
            prev_tags = cur_counts.get_word_tags(prev_word)

            # add to raw counts if new
            if word not in new_counts.t_words and word not in new_counts.r_words:
                new_counts.r_words.append(word)
                new_counts.num_words += 1

            # add to number of words seen
            new_counts.total_count += 1

            # iterate through tags for this word
            for tag in cur_tags:
                tag_ind = tag_inds[tag]
                tag_prob = tag_probs[tag_ind, i]

                if tag not in new_counts.t_counts:
                    new_counts.t_counts[tag] = 0

                # add expected number of times we see this tag
                new_counts.t_counts[tag] += np.exp(tag_prob)

                wt_pair = WT_Pair(word, tag)
                if wt_pair not in new_counts.tw_counts:
                    new_counts.tw_counts[wt_pair] = 0
                # add expected number of times we see this wt pair
                new_counts.tw_counts[wt_pair] += np.exp(tag_prob)

                # iterate through tags for previous word
                for prev_tag in prev_tags:

                    # get the prob of this tt pair and add to counts
                    tt_pair = TT_Pair(prev_tag, tag)
                    prev_tag_ind = tag_inds[prev_tag]

                    tt_prob = f_probs[prev_tag_ind, i - 1] + b_probs[tag_ind, i]
                    tt_prob += (cur_counts.get_wt_prob(word, tag) - Z) + cur_counts.get_tt_prob(prev_tag, tag)

                    if tt_pair not in new_counts.tt_counts:
                        new_counts.tt_counts[tt_pair] = 0

                    new_counts.tt_counts[tt_pair] += np.exp(tt_prob)

        # end of iteration, copy new counts into current, start a new new
        cur_counts = copy.deepcopy(new_counts)
        new_counts = copy.deepcopy(init_counts)

        # calculate and report new perplexity of untagged raw data
        perp = -1 * Z / (len(raw_seq) - 1)

        print(f"Iteration {n}:  Model perplexity per untagged raw word: {np.exp(perp):.4f}")

        # calculate accuracy of tagging test with viterbi
        calc_seq = viterbi(test_seq, cur_counts)
        known_acc, new_acc, total_acc, seen_acc = calc_accuracy(test_tags,
                calc_seq, test_seq, cur_counts, True)
        print(f"Tagging accuracy (Viterbi decoding): {100 * total_acc:.2f}%", end="")
        print(f"\t(known: {100 * known_acc:.2f}%  seen: {100 * seen_acc:.2f}%", end="")
        print(f"  novel: {100 * new_acc:.2f}%)")


    # finished EM, last posterior decoding using final counts
    f_probs, b_probs = forward_backward(test_seq, cur_counts,
                                            all_tags, tag_inds, all_words)

    result = []

    for i in range(len(test_seq)):
        result.append(all_tags[np.argmax(f_probs[:, i] + b_probs[:, i])])

    # return tags for test sequence and pray
    return result


def main():

    # parse args
    args = parse_args()

    # parse train/test/raw data
    tag_counts, seq, true_tags = io.parse_train_data(args.train_data, 1)
    test_seq, test_tags = io.parse_test_data(args.test_data)
    raw_seq = io.parse_raw_data(args.raw_data)

    # use EM and then tag test data
    calc_seq = em_tagging(test_seq, test_tags, tag_counts, raw_seq)
    # write output to file and yeet on out
    io.write_output(test_seq, calc_seq)
    

if __name__ == "__main__":
    main()
