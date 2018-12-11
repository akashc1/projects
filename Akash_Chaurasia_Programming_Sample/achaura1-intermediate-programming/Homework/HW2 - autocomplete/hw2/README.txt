Akash Chaurasia
601.220 (02)
09/14/2017
Homework 2
achaura1
achaura1@jhu.edu

1.)
To compile the program (named autocomplete.c) please enter the following command:
gcc autocomplete.c -std=c99 -Wall -Wextra -pedantic -o ac

The -o ac is just to make sure that the executable file is named "ac", as specified in the assignment document.

Please note that the sorting algorithm is quite slow, so entering any input right after entering "./ac" may cause a crash.
The program will enter a new line character to indicate when the sort is indicated, and that it is ready for input. PLEASE
wait for this new line before typing input. Thanks!


2.)
One challenge I ecnountered was implementing a sorting algorithm. I initially went with quicksort, since I read that it is
one of the most efficient sorting algorithms. However, I had a bit of trouble with the full implementation and then went with
bubble sort (slow as it may be) as it was very easy to implement.

I chose to modularize many parts of the program (almost every part), such as swapping two elements of the queries and weights
arrays (since they have to be swapped in parallel), searching, sorting, and printing the results.

3.)
One weight-based autocomplete program could use past prompts as part of the weighting system, as Google and many other search
engines probably do. This would involve recording every prompt ever input by the user and adding it to the queries with a special
marker (maybe a third weight array with the number of times the user has search that term). This would personalize the search
process.
