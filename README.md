# Introduction
This assignment was specified in the Stanford CS107 course, [Assignment 3](https://see.stanford.edu/materials/icsppcs107/11-Assignment-3-Vector.pdf). I implemented the std::vector interface found in C++ in pure C, allowing the use of generics/templates by using an elemSize parameter to denote the type.
Hence, one can instantiate a vector of type int of initial length 10 as follows:
```
vector int_vec;
VectorNew(&int_vec, sizeof(int), NULL, 10);
```
For more details, please see `vector.h`.

The test suite `vectortest.c` and the header file `vector.h` were provided beforehand. I am only responsible for the implementation.

