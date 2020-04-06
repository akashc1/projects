#!/usr/bin/env python3
# Module to estimate n-gram probabilities.



import logging
import math
import re
import sys
import numpy as np
import numpy.matlib

from pathlib import Path
from typing import Any, Counter, Dict, List, Optional, Set, Tuple, Union

log = logging.getLogger(Path(__file__).stem)  # Basically the only okay global variable.

Zerogram = Tuple[()]
Unigram = Tuple[str]
Bigram = Tuple[str, str]
Trigram = Tuple[str, str, str]
Ngram = Union[Zerogram, Unigram, Bigram, Trigram]
Vector = List[float]


BOS = "BOS"  # special word type for context at Beginning Of Sequence
EOS = "EOS"  # special word type for observed token at End Of Sequence
OOV = "OOV"  # special word type for all Out-Of-Vocabulary words
OOL = "OOL"  # special word type for all Out-Of-Lexicon words
OOV_THRESHOLD = (
    3
)  # minimum number of occurrence for a word to be considered in-vocabulary


def get_tokens(file: Path):
    """Iterate over the tokens, saving a few layers of nesting."""
    with open(file) as corpus:
        for line in corpus:
            for z in line.split():
                yield z  # This function is a generator.
                # It has `yield` instead of `return`, because
                # it provides each next piece of the output
                # while saving its current state.
                # You can iterate over this function:
                # for token in get_tokens(my_file):
                #     process_the_token(token)
    yield EOS  # Every file implicitly ends with EOS.


class LanguageModel:
    def __init__(self):
        self.tokens: Counter[Ngram] = Counter()  # the c(...) function.
        self.vocab: Optional[Set[str]] = None
        self.progress = 0  # To print progress.

    @classmethod
    def make(cls, smoother: str, lexicon: Path) -> "LanguageModel":
        """Factory pattern: Build the language model you need."""
        r = re.compile("^(.*?)-?([0-9.]*)$")
        m = r.match(smoother)

        lambda_: Optional[float]  # Type annotation only.

        if m is None or not m.lastindex:
            raise ValueError(f"Smoother regular expression failed for {smoother}")
        else:
            smoother_name = m.group(1).lower()
            if m.lastindex >= 2 and len(m.group(2)):
                lambda_arg = m.group(2)
                lambda_ = float(lambda_arg)
            else:
                lambda_ = None

        if lambda_ is None and smoother_name.find("add") != -1:
            raise ValueError(
                f"You must include a non-negative lambda value in smoother name {smoother}"
            )

        if smoother_name == "uniform":
            return UniformLanguageModel()
        elif smoother_name == "add":
            assert lambda_ is not None
            return AddLambdaLanguageModel(lambda_)
        elif smoother_name == "backoff_add":
            assert lambda_ is not None
            return BackoffAddLambdaLanguageModel(lambda_)
        elif smoother_name == "loglinear":
            assert lambda_ is not None
            return LogLinearLanguageModel(lambda_, lexicon)
        elif smoother_name == "improved":
            assert lambda_ is not None
            return ImprovedLogLinearLanguageModel(lambda_, lexicon)
        else:
            raise ValueError(f"Don't recognize smoother name {smoother_name}")

    def file_log_prob(self, corpus: Path) -> float:
        """Compute the log probability of the sequence of tokens in file.
        NOTE: we use natural log for our internal computation.  You will want to
        divide this number by log(2) when reporting log probabilities.
        """
        log_prob = 0.0
        x, y = BOS, BOS
        for z in get_tokens(corpus):
            prob = self.prob(x, y, z)
            log_prob += math.log(prob)
            x, y = y, z  # Shift over by one position.
        return log_prob

    def set_vocab_size(self, *files: Path) -> None:
        if self.vocab is not None:
            log.warning("Warning: vocabulary already set!")

        word_counts: Counter[str] = Counter()  # count of each word

        for file in files:
            for token in get_tokens(file):
                word_counts[token] += 1
                self.show_progress()

        vocab: Set[str] = set(w for w in word_counts if word_counts[w] >= OOV_THRESHOLD)
        vocab |= {  # Union equals
            OOV,
            EOS,
        }  # add EOS to vocab (but not BOS, which is never a possible outcome but only a context)

        self.vocab = vocab
        print(f"Vocabulary size is {self.vocab_size} types including OOV and EOS")

    @property
    def vocab_size(self) -> int:
        assert self.vocab is not None
        return len(self.vocab)

    def count(self, x: str, y: str, z: str) -> None:
        """Count the n-grams.  In the perl version, this was an inner function.
        For now, I am just using a data member to store the found tri-
        and bigrams.
        """
        self._count_ngram((x, y, z))
        self._count_ngram((y, z))
        self._count_ngram((z,))
        self._count_ngram(())

    def _count_ngram(self, ngram: Ngram) -> None:
        """Count the n-gram; that is, increment its count in the model."""
        self.tokens[ngram] += 1

    def num_tokens(self, corpus: Path) -> int:
        """Give the number of tokens in the corpus, including EOS."""
        return sum(1 for token in get_tokens(corpus))

    def prob(self, x: str, y: str, z: str) -> float:
        """Computes a smoothed estimate of the trigram probability p(z | x,y)
        according to the language model.
        """
        class_name = type(self).__name__
        if class_name == LanguageModel.__name__:
            raise NotImplementedError("Reimplement this in subclasses!")
        raise NotImplementedError(
            f"{class_name} is not implemented yet. (That's your job!)"
        )

    @classmethod
    def load(cls, source: Path) -> "LanguageModel":
        import pickle

        with open(source, mode="rb") as f:
            return pickle.load(f)

    def save(self, destination: Path) -> None:
        import pickle

        with open(destination, mode="wb") as f:
            pickle.dump(self, f, protocol=pickle.HIGHEST_PROTOCOL)

    def replace_missing(self, token: str) -> str:
        assert self.vocab is not None
        if token not in self.vocab:
            return OOV
        return token

    def train(self, corpus: Path) -> List[str]:
        """Read the training corpus and collect any information that will be needed
        by the prob function later on.  Tokens are whitespace-delimited.

        Note: In a real system, you wouldn't do this work every time you ran the
        testing program. You'd do it only once and save the trained model to disk
        in some format.
        """
        print(f"Training from corpus {corpus}")
        # The real work:
        # accumulate the type and token counts into the global hash tables.

        # If vocab size has not been set, build the vocabulary from training corpus
        if self.vocab is None:
            self.set_vocab_size(corpus)

        # Clear out any previous training.
        self.tokens = Counter()

        # We save the corpus in memory to a list tokens_list.  Notice that we
        # prepended two BOS at the front of the list and appended an EOS at the end.  You
        # will need to add more BOS tokens if you want to use a longer context than
        # trigram.
        x, y = BOS, BOS  # Previous two words.  Initialized as "beginning of sequence"
        # count the BOS context

        self.tokens[(x, y)] = 1
        self.tokens[(y,)] = 1  # The syntax for a 1-element tuple in Python

        tokens_list = [x, y]  # the corpus saved as a list
        for z in get_tokens(corpus):
            z = self.replace_missing(z)
            self.count(x, y, z)
            self.show_progress()
            x, y = y, z  # Shift over by 1 word.
            tokens_list.append(z)
        sys.stderr.write("\n")  # done printing progress dots "...."
        return tokens_list

    def show_progress(self, freq: int = 5000) -> None:
        """Print a dot to stderr every 5000 calls (frequency can be changed)."""
        self.progress += 1
        if self.progress % freq == 1:
            sys.stderr.write(".")


class UniformLanguageModel(LanguageModel):
    def prob(self, x: str, y: str, z: str) -> float:
        return 1 / self.vocab_size


class AddLambdaLanguageModel(LanguageModel):
    def __init__(self, lambda_: float) -> None:
        super().__init__()

        if lambda_ < 0:
            log.error(f"Lambda value was {lambda_}")
            raise ValueError(
                "You must include a non-negative lambda value in smoother name"
            )
        self.lambda_ = lambda_

    def prob(self, x: str, y: str, z: str) -> float:
        assert self.vocab is not None
        x = self.replace_missing(x)
        y = self.replace_missing(y)
        z = self.replace_missing(z)
        # Notice that summing the numerator over all values of typeZ
        # will give the denominator.  Therefore, summing up the quotient
        # over all values of typeZ will give 1, so sum_z p(z | ...) = 1
        # as is required for any probability function.
        return (self.tokens[x, y, z] + self.lambda_) / (
            self.tokens[x, y] + self.lambda_ * self.vocab_size
        )


class BackoffAddLambdaLanguageModel(LanguageModel):
    def __init__(self, lambda_: float) -> None:
        super().__init__()

        if lambda_ < 0:
            log.error(f"Lambda value was {lambda_}")
            raise ValueError(
                "You must include a non-negative lambda value in smoother name"
            )
        self.lambda_ = lambda_

    def prob(self, x: str, y: str, z: str) -> float:
        x = self.replace_missing(x)
        y = self.replace_missing(y)
        z = self.replace_missing(z)

        lV = self.lambda_ * self.vocab_size
        p_z = (self.tokens[(z,)] + lV) / (self.tokens[()] + lV)
        p_zgy = (self.tokens[(y, z)] + (lV * p_z)) / (self.tokens[(y,)] + lV)
        return (self.tokens[(x, y, z)] + (lV * p_zgy)) / (self.tokens[(x, y)] + lV)


class LogLinearLanguageModel(LanguageModel):
    def __init__(self, c: float, lexicon: Path) -> None:
        super().__init__()

        if c < 0:
            log.error(f"C value was {c}")
            raise ValueError("You must include a non-negative c value in smoother name")
        self.c: float = c
        self.vectors: Dict[str, Vector]
        self.dim: int
        self.vectors, self.dim = self._read_vectors(lexicon)

        self.X: Any = None
        self.Y: Any = None

    def _read_vectors(self, lexicon: Path) -> Tuple[Dict[str, Vector], int]:
        """Read word vectors from an external file.  The vectors are saved as
        arrays in a dictionary self.vectors.
        """
        with open(lexicon) as f:
            header = f.readline()
            dim = int(header.split()[-1])
            vectors: Dict[str, Vector] = {}
            for line in f:
                word, *arr = line.split()
                vectors[word] = [float(x) for x in arr]

        return vectors, dim

    def replace_missing(self, token: str) -> str:
        # substitute out-of-lexicon words with OOL symbol 
        assert self.vocab is not None
        if token not in self.vocab:
            token = OOV
        if token not in self.vectors:
            token = OOL
        return token

    def prob(self, x: str, y: str, z: str) -> float:
        assert self.vocab is not None
        assert self.vectors is not None
        x = self.replace_missing(x)
        y = self.replace_missing(y)
        z = self.replace_missing(z)

        x_vec = np.array(self.vectors[x])
        y_vec = np.array(self.vectors[y])
        z_vec = np.array(self.vectors[z])

        feat_1 = np.matmul(np.matmul(np.transpose(x_vec), self.X), z_vec)
        feat_2 = np.matmul(np.matmul(np.transpose(y_vec), self.Y), z_vec)

        u_xyz = feat_1 + feat_2

        E = []
        for v in sorted(self.vocab):
            v = self.replace_missing(v)
            v_vec = np.array(self.vectors[v])
            E.append(v_vec)

        E = np.transpose(E)

        x_norm = np.matmul(np.matmul(np.transpose(x_vec), self.X), E)
        y_norm = np.matmul(np.matmul(np.transpose(y_vec), self.Y), E)

        norm = np.sum(np.exp(x_norm + y_norm))


        return np.exp(u_xyz) / norm


    def train(self, corpus: Path) -> List[str]:
        """Read the training corpus and collect any information that will be needed
        by the prob function later on.  Tokens are whitespace-delimited.

        Note: In a real system, you wouldn't do this work every time you ran the
        testing program. You'd do it only once and save the trained model to disk
        in some format.
        """
        tokens_list = super().train(corpus)
        # Train the log-linear model using SGD.

        # Initialize parameters
        self.X = [[0.0 for _ in range(self.dim)] for _ in range(self.dim)]
        self.Y = [[0.0 for _ in range(self.dim)] for _ in range(self.dim)]

        # Optimization hyperparameters
        gamma0 = 0.01  # initial learning rate, used to compute actual learning rate
        epochs = 10  # number of passes

        self.N = len(tokens_list) - 2  # number of training instances
        # ******** COMMENT *********
        # In log-linear model, you will have to do some additional computation at
        # this point.  You can enumerate over all training trigrams as following.
        #
        # for i in range(2, len(tokens_list)):
        #   x, y, z = tokens_list[i - 2], tokens_list[i - 1], tokens_list[i]
        #
        # Note1: self.c is the regularizer constant C
        # Note2: You can use self.show_progress() to log progress.
        # **************************


        E = []
        for v in sorted(self.vocab):
            v = self.replace_missing(v)
            v_vec = np.array(self.vectors[v])
            E.append(v_vec)

        E = np.transpose(E)

        gamma = gamma0
        t = 0
        for a in range(epochs):
            F = 0.0
            for i in range(2, len(tokens_list)):

                x, y, z = tokens_list[i - 2], tokens_list[i - 1], tokens_list[i] 
                gamma = (gamma0)/(1 + (gamma0/self.N) * 2 * self.c * t)
        
                x = self.replace_missing(x)
                y = self.replace_missing(y)
                z = self.replace_missing(z)

                x_vec = np.array(self.vectors[x])
                y_vec = np.array(self.vectors[y])
                z_vec = np.array(self.vectors[z])

                x_norm = np.matmul(np.matmul(np.transpose(x_vec), self.X), E)
                y_norm = np.matmul(np.matmul(np.transpose(y_vec), self.Y), E)
                
                U = np.exp(x_norm + y_norm)
                norm = np.sum(np.exp(x_norm + y_norm))
                U = U / norm
                X_update = np.zeros((self.dim, self.dim))
                Y_update = np.zeros((self.dim, self.dim))

                for j in range(self.dim):
                    x_regularization = 2 * self.c * np.array(self.X[j]) / self.N 
                    y_regularization = 2 * self.c * np.array(self.Y[j]) / self.N 
                    
                    X_update[j] += x_vec[j] * np.transpose(z_vec)
                    X_update[j] -= x_vec[j] * np.matmul(np.matlib.repmat(U, self.dim, 1), E.T)[j] + x_regularization

                    Y_update[j] += y_vec[j] * np.transpose(z_vec)
                    Y_update[j] -= y_vec[j] * np.matmul(np.matlib.repmat(U, self.dim, 1), E.T)[j] + y_regularization

                self.X += gamma * X_update
                self.Y += gamma * Y_update
                t += 1

                F += np.log(self.prob(x, y, z))
                F -= (self.c / self.N) * (np.sum(np.multiply(self.X, self.X)) + np.sum(np.multiply(self.Y, self.Y)))

            F /= self.N

            print(f"epoch {a + 1}: F={F:.5f}")

        print(f"Finished training on {self.tokens[()]} tokens")
        return tokens_list  # Not really needed, except to obey typing.


class ImprovedLogLinearLanguageModel(LanguageModel):
    def __init__(self, c: float, lexicon: Path) -> None:
        super().__init__()

        if c < 0:
            log.error(f"C value was {c}")
            raise ValueError("You must include a non-negative c value in smoother name")
        self.c: float = c
        self.vectors: Dict[str, Vector]
        self.dim: int
        self.vectors, self.dim = self._read_vectors(lexicon)

        self.X: Any = None
        self.Y: Any = None

    def _read_vectors(self, lexicon: Path) -> Tuple[Dict[str, Vector], int]:
        """Read word vectors from an external file.  The vectors are saved as
        arrays in a dictionary self.vectors.
        """
        with open(lexicon) as f:
            header = f.readline()
            dim = int(header.split()[-1])
            vectors: Dict[str, Vector] = {}
            for line in f:
                word, *arr = line.split()
                vectors[word] = [float(x) for x in arr]

        return vectors, dim

    def replace_missing(self, token: str) -> str:
        # substitute out-of-lexicon words with OOL symbol 
        assert self.vocab is not None
        if token not in self.vocab:
            token = OOV
        if token not in self.vectors:
            token = OOL
        return token

    def prob(self, x: str, y: str, z: str) -> float:
        assert self.vocab is not None
        assert self.vectors is not None
        x = self.replace_missing(x)
        y = self.replace_missing(y)
        z = self.replace_missing(z)

        x_vec = np.array(self.vectors[x])
        y_vec = np.array(self.vectors[y])
        z_vec = np.array(self.vectors[z])
        tri_vec = np.zeros((self.dim, self.dim, self.dim))

        x_y = np.outer(x_vec, y_vec.T)

        for i in range(self.dim):
            tri_vec[:, :, i] = np.multiply(x_y, z_vec[i])

        t_feats = np.sum(np.multiply(self.T, tri_vec))

        feat_1 = np.matmul(np.matmul(np.transpose(x_vec), self.X), z_vec)
        feat_2 = np.matmul(np.matmul(np.transpose(y_vec), self.Y), z_vec)
        feat_uni_x = np.matmul(np.transpose(x_vec), self.unigram_X)
        feat_uni_y = np.matmul(np.transpose(y_vec), self.unigram_Y)
        feat_uni_z = np.matmul(np.transpose(z_vec), self.unigram_Z)

        u_xyz = feat_1 + feat_2 + feat_uni_x + feat_uni_y + feat_uni_z + t_feats

        E = []
        for v in sorted(self.vocab):
            v = self.replace_missing(v)
            v_vec = np.array(self.vectors[v])
            E.append(v_vec)

        E = np.transpose(E)

        T_norm_vec = np.zeros((self.dim, self.dim, self.dim, E.shape[1]))
        x_norm = np.matmul(np.matmul(np.transpose(x_vec), self.X), E)
        y_norm = np.matmul(np.matmul(np.transpose(y_vec), self.Y), E)

        for l in range(E.shape[1]):

            for k in range(self.dim):
                T_norm_vec[:, :, k, l] = np.multiply(x_y, E[k, l])

            T_norm_vec[:, :, :, l] = np.multiply(self.T, T_norm_vec[:, :, :, l])

        unigram_x_norm = np.matmul(np.transpose(x_vec), self.unigram_X)
        unigram_y_norm = np.matmul(np.transpose(y_vec), self.unigram_Y)
        unigram_z_norm = np.matmul(self.unigram_X, E)

        repeated_x_uni_norm = np.ones(E.shape[1]) * unigram_x_norm
        repeated_y_uni_norm = np.ones(E.shape[1]) * unigram_y_norm

        T_norm = np.sum(T_norm_vec, axis=(0, 1, 2))

        norm = np.sum(np.exp(x_norm + y_norm + repeated_x_uni_norm + repeated_y_uni_norm + unigram_z_norm + T_norm))

        return np.exp(u_xyz) / norm


    def train(self, corpus: Path) -> List[str]:
        """Read the training corpus and collect any information that will be needed
        by the prob function later on.  Tokens are whitespace-delimited.

        Note: In a real system, you wouldn't do this work every time you ran the
        testing program. You'd do it only once and save the trained model to disk
        in some format.
        """
        tokens_list = super().train(corpus)
        # Train the log-linear model using SGD.

        # Initialize parameters
        self.X = [[0.0 for _ in range(self.dim)] for _ in range(self.dim)]
        self.Y = [[0.0 for _ in range(self.dim)] for _ in range(self.dim)]
        self.unigram_X = [0.0 for _ in range(self.dim)]
        self.unigram_Y = [0.0 for _ in range(self.dim)]
        self.unigram_Z = [0.0 for _ in range(self.dim)]
        self.T = np.zeros((self.dim, self.dim, self.dim))

        # Optimization hyperparameters
        gamma0 = 0.001  # initial learning rate, used to compute actual learning rate
        epochs = 10  # number of passes

        self.N = len(tokens_list) - 2  # number of training instances
        # ******** COMMENT *********
        # In log-linear model, you will have to do some additional computation at
        # this point.  You can enumerate over all training trigrams as following.
        #
        # for i in range(2, len(tokens_list)):
        #   x, y, z = tokens_list[i - 2], tokens_list[i - 1], tokens_list[i]
        #
        # Note1: self.c is the regularizer constant C
        # Note2: You can use self.show_progress() to log progress.
        # **************************


        E = []
        for v in sorted(self.vocab):
            v = self.replace_missing(v)
            v_vec = np.array(self.vectors[v])
            E.append(v_vec)

        E = np.transpose(E)

        gamma = gamma0
        t = 0
        for i in range(epochs):
            print(f'EPOCH: {i + 1}')
            train_set = np.random.permutation(range(2, len(tokens_list)))
            for i in train_set:

                x, y, z = tokens_list[i - 2], tokens_list[i - 1], tokens_list[i] 
                gamma = (gamma0)/(1 + (gamma0/self.N) * 2 * self.c * t)
        
                x = self.replace_missing(x)
                y = self.replace_missing(y)
                z = self.replace_missing(z)

                x_vec = np.array(self.vectors[x])
                y_vec = np.array(self.vectors[y])
                z_vec = np.array(self.vectors[z])

                x_y = np.outer(x_vec, y_vec)
                T_norm_vec = np.zeros((self.dim, self.dim, self.dim, E.shape[1]))
                tri_vec = np.zeros((self.dim, self.dim, self.dim))
                all_xyz = T_norm_vec.copy()

                # outer product
                for k in range(self.dim):
                    tri_vec[:, :, k] = np.multiply(x_y, z_vec[k])
                
                for l in range(E.shape[1]):
                    # d x d x d x V
                    for k in range(self.dim):
                        T_norm_vec[:, :, k, l] = np.multiply(x_y, E[k, l])


                    all_xyz[:, :, :, l] = T_norm_vec[:, :, :, l]
                    T_norm_vec[:, :, :, l] = np.multiply(self.T, T_norm_vec[:, :, :, l])


                x_norm = np.matmul(np.matmul(np.transpose(x_vec), self.X), E)
                y_norm = np.matmul(np.matmul(np.transpose(y_vec), self.Y), E)

                unigram_x_norm = np.matmul(np.transpose(x_vec), self.unigram_X)
                unigram_y_norm = np.matmul(np.transpose(y_vec), self.unigram_Y)
                unigram_z_norm = np.matmul(self.unigram_X, E)

                repeated_x_uni_norm = np.ones(E.shape[1]) * unigram_x_norm
                repeated_y_uni_norm = np.ones(E.shape[1]) * unigram_y_norm

                T_norm = np.sum(T_norm_vec, axis=(0, 1, 2))

                Us = np.exp(x_norm + y_norm + repeated_x_uni_norm + repeated_y_uni_norm + unigram_z_norm + T_norm)
                norm = np.sum(Us)
                Ps = Us / norm

                X_update = np.zeros((self.dim, self.dim))
                Y_update = np.zeros((self.dim, self.dim))

                uni_x_regularization = 2 * self.c * np.array(self.unigram_X) / self.N 
                uni_y_regularization = 2 * self.c * np.array(self.unigram_Y) / self.N 
                uni_z_regularization = 2 * self.c * np.array(self.unigram_Z) / self.N 

                uni_x_update = np.zeros(self.dim)
                uni_y_update = np.zeros(self.dim)
                uni_z_update = np.zeros(self.dim)

                T_update = np.zeros((self.dim, self.dim, self.dim, E.shape[1]))

                T_regularization = 2 * self.c * self.T / self.N

                uni_x_update += x_vec - x_vec * np.sum(Ps) - uni_x_regularization
                uni_y_update += y_vec - y_vec * np.sum(Ps) - uni_y_regularization
                uni_z_update += x_vec - x_vec * np.sum(Ps) - uni_z_regularization

                for i, u in enumerate(Us):
                    T_update[:, :, :, i] -= u * all_xyz[:, :, :, i]

                T_update = np.sum(T_update, axis=3)
                T_update = np.divide(T_update, norm)

                T_update += tri_vec - T_regularization

                for j in range(self.dim):


                    x_regularization = 2 * self.c * np.array(self.X[j]) / self.N 
                    y_regularization = 2 * self.c * np.array(self.Y[j]) / self.N 


                    X_update[j] += x_vec[j] * np.transpose(z_vec)
                    X_update[j] -= x_vec[j] * np.matmul(np.matlib.repmat(Ps, self.dim, 1), E.T)[j] + x_regularization

                    Y_update[j] += y_vec[j] * np.transpose(z_vec)
                    Y_update[j] -= y_vec[j] * np.matmul(np.matlib.repmat(Ps, self.dim, 1), E.T)[j] + y_regularization

                self.X += gamma * X_update
                self.Y += gamma * Y_update
                self.unigram_X += gamma * uni_x_update
                self.unigram_Y += gamma * uni_y_update
                self.unigram_Z += gamma * uni_z_update
                self.T += gamma * T_update

                t += 1

        print(f"Finished training on {self.tokens[()]} tokens")
        return tokens_list  # Not really needed, except to obey typing.


