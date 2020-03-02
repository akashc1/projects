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
    
    def add(self, child):
        self.children.append(child)

    def get_child(self):
        index = random.randint(0, len(self.children) - 1)
        return self.children[index]

# recursive method to build sentences
# takes the dictionary and the key to use for the specific case
def generate_sentence(grammar, rule):
    sentence = ''
    child = grammar[rule].get_child()

    for next_rule in child:
        #non terminal
        if next_rule in grammar:
            sentence += generate_sentence(grammar, next_rule)
        #terminal
        else:
            sentence += next_rule + ' '

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
    l.pop(0) # ignore number
    rule = l[0]
    l.pop(0)
    child = l

    if rule in grammar_tree:
        grammar_tree[rule].add(child)
    else:
        grammar_tree[rule] = Rule(rule)
        grammar_tree[rule].add(child)

# execute for given number of iterations
for i in range(num_sentences):
    sentence = generate_sentence(grammar_tree, 'ROOT')
    print(sentence)
