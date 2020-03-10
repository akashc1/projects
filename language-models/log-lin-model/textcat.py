#!/usr/bin/env python3
"""
Akash Chaurasia
akashc@jhu.edu


Computes the log probability of the sequence of tokens in file,
according to a trigram model.  The training source is specified by
the currently open corpus, and the smoothing method used by
prob() is polymorphic.
"""

import argparse
import logging
from pathlib import Path

try:
    # Numpy is your friend. Not *using* it will make your program so slow.
    # So if you comment this block out instead of dealing with it, you're
    # making your own life worse.
    #
    # We made this easier by including the environment file in this folder.
    # Install Miniconda, then create and activate the provided environment.
    import numpy as np
except ImportError:
    print("\nERROR! Try installing Miniconda and activating it.\n")
    raise


from Probs import LanguageModel

TRAIN = "TRAIN"
TEST = "TEST"

log = logging.getLogger(Path(__file__).stem)  # Basically the only okay global variable.


def get_model_filename(smoother: str, lexicon: Path, train_file: Path) -> Path:
    return Path(f"{smoother}_{lexicon.name}_{train_file.name}.model")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(__doc__)
    parser.add_argument("mode", choices={TRAIN, TEST}, help="execution mode")
    parser.add_argument(
        "smoother",
        type=str,
        help="""Possible values: uniform, add1, backoff_add1, backoff_wb, loglinear1
  (the "1" in add1/backoff_add1 can be replaced with any real λ >= 0
   the "1" in loglinear1 can be replaced with any C >= 0 )
""",
    )
    parser.add_argument(
        "lexicon",
        type=Path,
        help="location of the word vector file; only used in the loglinear model",
    )
    parser.add_argument("train_files", type=Path,
                        help="location of the 2 training corpora (2 required)", nargs=2)

    # adding prior probability required for testing
    parser.add_argument("prior", type=float, help='''prior probability that a test file will be of the same
                                                    class as the first training corpus''', nargs='?')

    parser.add_argument("test_files", type=Path, nargs="*")


    verbosity = parser.add_mutually_exclusive_group()
    verbosity.add_argument(
        "-v",
        "--verbose",
        action="store_const",
        const=logging.DEBUG,
        default=logging.INFO,
    )
    verbosity.add_argument(
        "-q", "--quiet", dest="verbose", action="store_const", const=logging.WARNING
    )

    args = parser.parse_args()

    # Sanity-check the configuration.
    if args.mode == "TRAIN" and args.test_files:
        parser.error("Shouldn't see test files when training.")
    elif args.mode == "TEST" and not args.test_files:
        parser.error("No test files specified.")
    elif args.mode == "TEST" and not args.prior:
        parser.error("No prior probability provided.")
    elif args.mode == "TEST" and (args.prior < 0 or args.prior > 1):
        parser.error("Prior probability must be in the range [0, 1]")

    return args


def main():
    args = parse_args()
    logging.basicConfig(level=args.verbose)

    if args.mode == TRAIN:

        lms = [LanguageModel.make(args.smoother, args.lexicon) for _ in args.train_files]

        for i, lm in enumerate(lms):
            lm.set_vocab_size(args.train_files[0], args.train_files[1])
            lm.train(args.train_files[i])
            model_path = get_model_filename(args.smoother, args.lexicon,
                                            args.train_files[i])
            lm.save(destination=model_path)

    elif args.mode == TEST:
        lms = [LanguageModel.load(get_model_filename(args.smoother, args.lexicon,
                t_file)) for t_file in args.train_files]
        # We use natural log for our internal computations and that's
        # the kind of log-probability that fileLogProb returns.
        # But we'd like to print a value in bits: so we convert
        # log base e to log base 2 at print time, by dividing by log(2).
        model_enum = {0: args.train_files[0], 1: args.train_files[1]}
        file_classes = []
        file_truth = []
        right_count = 0
        total_count = 0

        total_log_prob = 0.0
        token_count = 0

        for test_file in args.test_files:

            # prob(file = type)
            prior1 = args.prior
            prior2 = 1.0 - prior1

            # should be log-proportional to p(type | file)
            log_map1 = lms[0].file_log_prob(test_file)
            log_map2 = lms[1].file_log_prob(test_file)

            if "loglinear" not in args.smoother or "improved" not in args.smoother:
                log_map1, log_map2 = (log_map1 + np.log(prior1)), (log_map2 + np.log(prior2))

            file_classes.append(0 if log_map1 > log_map2 else 1)
            if ((file_classes[-1] == 0 and '/english/' in str(test_file)) or
                    (file_classes[-1] == 1 and '/spanish/' in str(test_file))):
                right_count += 1
            total_count += 1

            total_log_prob += log_map1 + log_map2
            token_count += lms[0].num_tokens(test_file)

            # classify depending on which one has a higher prob (equiv. to higher log-prob)
            print(f"{model_enum[file_classes[-1]]}\t{test_file}")

        model2 = sum(file_classes) / len(file_classes)
        model1 = len(file_classes) - sum(file_classes)

        print(f"Total cross-entropy: {(total_log_prob / token_count):.4f}")

        print(f"Accuracy: {(100.0 * (right_count / total_count)):.2f}")
        print(f"{model1} files were more probably {model_enum[0]} ({100*(1 - model2):.2f}%)")
        print(f"{len(file_classes) - model1} files were more probably {model_enum[1]} ({100*model2:.2f}%)")

    else:
        raise ValueError("Inappropriate mode of operation.")


if __name__ == "__main__":
    main()

