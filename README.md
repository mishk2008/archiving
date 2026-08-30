# Analysis of data archiving algorithms

This is learning project (term paper) dedicated to the study of practical implementation of classical archiving algorithms in the C

## What's implemented

* **RLE (Run-Length Encoding)** - char repetition encoding algorithm. Good in files with long sequences of identical bytes
* **LZ77** - dictionary-based compression using a sliding window. Forms the basis of many modern archivers
* **Huffman(partially vibecode)** - entropy compression based on frequency analysis

## Project structure

* `main.c` - entry point, demonstration of the algorithm's operation
* `alg.h`  - common header file with function prototypes
* `algs/`  - a folder containing source code for specific algorithms, as well as for file and memory operations

## How to build and run

1. Clone the repository:

``git clone https://github.com/mishk2008/archiving.git``

2. Compile the files (example for GCC):

``gcc *.c algs/*.c -o archiver``

3. Run the program:

``./archiver``
