"""
Akash Chaurasia
akashc@jhu.edu
"""

import numpy as np
from typing import NamedTuple

# class for a word, tag pair
class WT_Pair(NamedTuple):
    word: str
    tag: str

# class for a tag, tag pair
class TT_Pair(NamedTuple):
    tag1: str
    tag2: str

'''
Class used to hold counts for any observations from
training and raw data

also delivers log-probabilities for any desired pairs
'''
class TagCounter:

    # constructor, lotta data to store
    def __init__(self, lamb=None):
        self.tt_counts = {}
        self.tw_counts = {}
        self.tag_dict = {}
        self.t_counts = {}
        self.w_counts = {}
        self.total_count = 0
        self._lambda = lamb
        self.num_wt_pairs = 0
        self.num_tt_pairs = 0
        self.num_tags = 0
        self.num_words = 1
        self.start_probs = []
        self.end_probs = []
        self.t_words = []
        self.r_words = []

    # adds tag, word, and prev_tag counts during training
    def add(self, word, tag, prev_tag=None):

        wt_pair = WT_Pair(word, tag)
        tt_pair = TT_Pair(prev_tag, tag)

        # total length of data seen
        self.total_count += 1

        # updating tag dictionary 
        if word not in self.tag_dict:
            self.tag_dict[word] = []

        # updating all words seen in training
        if word not in self.t_words:
            self.t_words.append(word)

        # adding tag to tag dictionary if needed
        if tag not in self.tag_dict[word]:
            self.tag_dict[word].append(tag)
        
        # updating all tag_counts
        if tag not in self.t_counts:
            self.t_counts[tag] = 0
            self.num_tags += 1

        # update number of times this tag is seen
        self.t_counts[tag] += 1

        # add tt_pair to counts
        if tt_pair not in self.tt_counts:
            self.tt_counts[tt_pair] = 0
            self.num_tt_pairs += 1
       
        # update transition count
        self.tt_counts[tt_pair] += 1

        # update word counts
        if word not in self.w_counts:
            self.w_counts[word] = 0
            self.num_words += 1

        self.w_counts[word] += 1

        # update emission count
        if wt_pair not in self.tw_counts:
            self.tw_counts[wt_pair] = 0
            self.num_wt_pairs += 1 

        self.tw_counts[wt_pair] += 1

    '''
    returns log-prob of emission of wt pair

    if a lambda is present, uses it to smooth
    '''
    def get_wt_prob(self, word, tag):

        wt_pair = WT_Pair(word, tag)

        # if no smoothing and haven't seen wt pair, p = 0
        if wt_pair not in self.tw_counts and self._lambda is None:
            return np.NINF
        
        num = 0
        denom = self.t_counts[tag]
        
        # necessary to get proper start/stop probabilities
        if denom == 0 and tag == '###':
            denom = 1

        if wt_pair in self.tw_counts:
            num = self.tw_counts[wt_pair]

        # add smoothing if we have a lambda
        if self._lambda is not None:
            num += self._lambda
            denom += self._lambda * self.num_words

        log_prob = np.log(num / denom)

        return log_prob

    '''
    returns a log-prob of transition for tt pair

    if a lambda is present, uses it to smooth
    '''
    def get_tt_prob(self, prev_tag, tag):

        tt_pair = TT_Pair(prev_tag, tag)

        # if tt pair never seen and no smoothing, p = 0
        if tt_pair not in self.tt_counts and self._lambda is None:
            return np.NINF

        num = 0
        denom = self.t_counts[prev_tag]

        # necessary to get proper start/stop probs
        if denom == 0 and prev_tag == '###':
            denom = 1

        if tt_pair in self.tt_counts:
            num = self.tt_counts[tt_pair]

        # add-lambda smoothing if given lambda
        if self._lambda is not None:
            num += self._lambda
            denom += self._lambda * self.num_tags

        log_prob = np.log(num / denom)

        return log_prob

    '''
    return probability of seeing tag overall
    '''
    def get_tag_prob(self, tag):

        # if never seen and no smoothing, p = 0
        if tag not in self.t_counts and self._lambda is None:
            return np.NINF

        if self.total_count == 0:
            return np.NINF

        num = 0
        denom = self.total_count

        if tag in self.t_counts:
            num += self.t_counts[tag]

        if self._lambda is not None:
            num += self._lambda
            denom += self._lambda * self.num_tags

        log_prob = np.log(num / denom)

        return log_prob

    '''
    gets the tags seen paired with a given word
    returns all tags except ### if haven't seen the word
    '''
    def get_word_tags(self, word):
        if word in self.tag_dict:
            return self.tag_dict[word]

        all_tags = list(self.t_counts.keys())
        all_tags.remove('###')

        return all_tags

    # returns if this word is from training
    def known_word(self, word):
        return word in self.t_words
    
    # returns if this word is from raw
    def seen_word(self, word):
        return word in self.r_words

    # gets a standardized tag list and hashed indices
    def get_count_tables(self):
        all_tags = list(self.t_counts.keys())
        all_words = list(self.w_counts.keys())
        tag_inds = {}

        # so we can use integers, slightly faster
        for i, tag in enumerate(all_tags):
            tag_inds[tag] = i

        return all_tags, all_words, tag_inds


