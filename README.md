RNA Stem Loop Visualizer
========================

Software for finding and visualizing RNA Stem Loops from a RNA sequence.

## Introduction
RNA (**R**ibo**N**ucleine **Acid**) is actually a chain of nucleotides,
abbreviated as G, A, U and C and where G is a complement of C and U of A and
vice versa. The RNA will form a stem loop if there are two parts in the RNA
sequence consisting of complements of each other (the stem) while there are
other non-complementing nucleotides in between (the loop).

### Basic Version
Assume a RNA sequence like `CCUGCXXXXXGCAGG`, then this software will parse
this sequence and testify whether this is a stem loop. If so, then it will
visualize something like below:
```
 C === G
 C === G
 U === A
 G === C
 C === G
X       X
 X     X
    X
```
Where the upperpart is a stem and the lower one is a loop.

We'll use a PDA (Push-Down Automaton for parsing the sequence and maybe
we'll extend it to using a LL or LR parser or Turing Machine instead).

> We'll assume that the loop starts where X does **not** match with a
> complementary X anymore (e.g. G cannot match with an A, so then the loop
> starts from there).

### Extensions
```
[ ] Input RNA sequence where the initial nucleotide is not necessarily the
    beginning of the stem loop.

[ ] Support sequences with multiple stem loops.

[ ] Input which elements should be in the loop, so you can track down
    specific stem loops.

[ ] Use something fancier than ASCII to visualize the stem loop.

[ ] Use LL or RL parser or Turing Machine instead of PDA.

[ ] GUI

[ ] ...
```

## Installing
Hey, we haven't even started the project yet!

## Usage
Hey, we haven't implemented this yet!

## Contributing
You are welcome! Just send a pull request.

Bugs, questions and/or ideas are welcome too, just put them on the [issues page](https://github.com/rubenvanassche/RNA-Stem-Loop-Visualizer/issues).
(Also: it's really cool if you've fixed that bug by yourself and send a PR
instead)
