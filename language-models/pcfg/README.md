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
