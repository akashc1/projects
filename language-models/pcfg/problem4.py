"""
Akash Chaurasia
akashc@jhu.edu
"""

import sys, random

##############################################################################
# Classes and Functions
#

# Rule class
class Rule:
    def __init__(self, _name):
        self.name = _name
        self.children = [] # list of rule names
        self.probs = [] # list of probabilities

    def add(self, child, prob):
        self.children.append(child)
        self.probs.append(prob)

    def get_child(self):
        rn = random.random()
        curprob = 0
        index = 0
        for prob in self.probs: 
            if curprob > rn:
                break
            index += 1
            curprob += prob
        return self.children[index - 1]

    def normalize_and_sort(self):
        probsum = sum(self.probs)
        for i in range(0, len(self.probs)):
            self.probs[i] /= probsum

        zcp = sorted(zip(self.probs, self.children))
        for i in range(0, len(zcp)):
            self.probs[i] = zcp[i][0]
            self.children[i] = zcp[i][1]

# recursive method to build sentences
# takes the dictionary and the key to use for the specific case
def generate_sentence(grammar, rule, nterms):
    sentence = '(' + rule + ' '
    child = grammar[rule].get_child()

    for next_rule in child:
        #non terminal
        if next_rule in grammar:
            if nterms < 450:
                nterms += 1
                sentence += generate_sentence(grammar, next_rule, nterms)
            else:
                sentence += "... "
        #terminal
        else:
            sentence += next_rule + " "

    sentence = sentence[:-1]
    sentence += ') '
    return sentence

#############################################################################
# Execution
#

# get filename, open file and retrieve lines
filename = sys.argv[1]
num_sentences = int(sys.argv[2])
g_file = open(filename, "r")
lines = g_file.readlines()
g_file.close()

# using dictionary to store all mappings
grammar_tree = {}
# split lines by whitespace, get rid of comments & line numbers
for line in lines:
    l = line.split()
    prob = float(l[0])
    l.pop(0)
    rule = l[0]
    l.pop(0)
    child = l

    if rule in grammar_tree:
        grammar_tree[rule].add(child, prob)
    else:
        grammar_tree[rule] = Rule(rule)
        grammar_tree[rule].add(child, prob)

# normalize probabilities
for rule in grammar_tree:
    grammar_tree[rule].normalize_and_sort()

# execute for given number of iterations
for i in range(num_sentences):
    sentence = generate_sentence(grammar_tree, 'ROOT', 1)
    print(sentence)

