RNA Stem Loop Visualizer
========================

Software for finding and visualizing RNA Stem Loops from a RNA sequence.

## Introduction
RNA (**R**ibo **N**ucleine **Acid**) is actually a chain of nucleotides,
abbreviated as G, A, U and C and where G is a complement of C and U of A.
The RNA will form a stem loop if there are two parts in the RNA
sequence consisting of complements of each other (the stem) while there are
other non-complementing nucleotides in between (the loop).

Assume a RNA sequence like `CCUGCXXXXXGCAGG`, then this software will parse
this sequence and test whether this is a stem loop. If so, then it will
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
Where the upper part is a stem and the lower one is a loop.

Either a CFG, an LL Parser or a Turing Machine (up to the user)
is used to analyze the string.

Note that not all algorithms will give the same result. They all look for
something slightly different.

CFG: A stem loop across the entire input string

LL Parser: A stem loop that does not necessarily use the entire input string.
If there are multiple stem loops, the one with the longest stem is chosen.
If there are multiple stem loops with the maximum size, the leftmost one is chosen.

Turing Machine: Looks for the same stem loops as the LL Parser, 
however only stem loops with loops of at least 2 nucleotides will be considered.

A GUI is provided in the RNAStemLoop executable. Enter an input string, 
choose an algorithm, press "Analyze" to analyze a string and press "Visual RSL"
to generate a visualization of the stem loop. Remember to re-analyze after
changing any input!

## Installing
```sh
# assuming you are now in the project root directory
$ mkdir build
$ cd build
$ cmake ..
$ make 
$ make install
```

Note that Qt4 and SFML2.0 are required!

Now you'll get the following executables in the `bin/` directory:

### Executables

- `RNAStemLoop` [The GUI for the RNA Stem Loop Visualizer]
- `RNA-Stem-Loop-Visualizer` [The visualizer, shouldn't be called by the user]
- `Tests` Runs all the tests using the [Catch](https://github.com/philsquared/Catch) 
  testing environment.


## Usage
Open RNAStemLoopVisualizer and start visualizing!

## Documentation
The API Documentation can be made by changing the BUILD_DOCS bool in the CMakeLists.txt file.
We also provide an online version of the documentation which can be found here: http://rubenvanassche.github.io/RNA-Stem-Loop-Visualizer/docs/html/
A PDF version can also be found in the /docs/ folder

## Made By
```
Pieter Lauwers
Jakob Struye
Ruben Van Assche
Stijn Wouters
```

## Contributing
You are welcome! Just send a pull request.

Bugs, questions and/or ideas are welcome too, just put them on the [issues page](https://github.com/rubenvanassche/RNA-Stem-Loop-Visualizer/issues).
(Also: it's really cool if you've fixed that bug by yourself and send a PR
instead)



Note that the following sections are not about the RNA Stem Loop Visualizer!
They only concern testing of some algorithms used by the Visualizer.
### Workshop
There are also some executables and sample files that was provided during the 
workshop sessions in University of Antwerp:

(sample files are put in the `workshop` folder)

#### `RunTuring` - Turing Machines (Jakob Struye)
```
    INPUT 
Provide the name of a Turing Machine in .xml-format (including the ".xml" 
suffix) as a command line argument. When prompted, enter an input string

    OUTPUT
"String accepted" or "String not accepted" for every input string entered 
(or an exception message)

    EXAMPLES
Two Turing Machines in .xml format are provided as examples:

    - TMInput1.xml shows the basic structure of the .xml file
    - TMInput2.xml is a fully functional TM using both storage in states 
      and multitrack and accepts all strings wcw (w being any string 
      consisting of 0s and 1s of at least length 1)
```

#### `RunCYK` - CFG / CNF / CYK-algorithm (Stijn Wouters)
```
    INPUT
An CFG or CNF (at your choice) in .xml-format (including the ".xml" suffix)
and a string as a command line argument.

    OUTPUT
A fancy, green colored "YES!" when the passed string is in the language of
the CFG/CNF defined from the xml-file, otherwise you'll get an (also fancy)
red colored "NO!". In case of an invalid CFG/CNF construction you'll get an 
error message.

    NOTE
If you choose a CFG as input, then keep in mind that it will be converted
to an CNF first before the program will actually run the CYK algorithm.

    EXAMPLE
Three examples are provided, each with different structure/syntaxes:

    - CYK_SampleInput0.xml is a CFG accepting all strings with even a's or
      even b's.
    - CYK_SampleInput1.xml is a CFG accepting strings with odd a's or b's.
    - CYK_SampleInput2.xml is a CFG recognizing palindrome-like strings.
```

#### `RunPDA` - PDA (Ruben Van Assche)
```
    INPUT 
Provide the name of a PDA in .xml-format (including the ".xml" 
suffix) as a command line argument. When prompted, enter an input string

    OUTPUT
"String accepted" or "String not accepted" for every input string entered 
(or an exception message)

    EXAMPLES
One ODA in .xml format is provided as example:

    - PDAInput1.xml is a PDA accepting all the palindromes of the form : [0+1]*c[0+1]*
```

#### LLParser (Pieter Lauwers)
```
    INPUT
Provide a grammar in the form of the given example file. The grammar has to be a legal LL(k) grammar. 

    OUTPUT
The parse table for the provided grammar.

    EXAMPLES
LLParserInput.txt results in LLParserOutput.txt
```
