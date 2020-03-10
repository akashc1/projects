### Probabilistic Context-free Grammar (PCFG)

This exercise was to explore how PCFGs can describe a language model.

I first started with a simple CFG with no weights:

```
...
# Rules for creating full sentences.
1	ROOT	S .
1	ROOT	S !
1	ROOT	is it true that S ?     # mixing terminals and nonterminals is ok.
...

# Nonterminal rules, basic sentence structure.
1	VP	Verb NP
1	PP	Prep NP
1	Noun	Adj Noun

...

# Terminals, basically actual vocabulary.
1	Verb	pickled

1	Det	the

1	Noun	president

1	Adj	perplexed

1	Prep	on
...

```

I created a program called `randsent` which uses the CFG to create sentences using a uniform probability distribution to select rules. 

The numbers at the beginning of each line provide a weighting I implemented, with a larger weight resulting in a larger probability that that specific rule is chosen (out of all rules with the same parent ie. ROOT). All sentences are chosen to begin with ROOT, since this is an assumption we can make.

I then played around with the weights to make it more _probabilistic_ - this was especially necessary because I included some recursive rules (NP -> NP, PP), and I wanted to make this more realistic.
