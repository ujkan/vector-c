# Introduction
This assignment was specified in the Stanford CS107 course, [Assignment 3](https://see.stanford.edu/materials/icsppcs107/11-Assignment-3-Vector.pdf). I implemented the `std::vector` interface found in C++ in pure C, circumventing the use of generics/templates, which are unavailable in C, by passing a parameter called `elemSize` to denote the type (by its size).
For example, one can instantiate a vector of type int of initial length 10 as follows:
```c
vector int_vec;
VectorNew(&int_vec, sizeof(int), NULL, 10);
```
For more details, please see `vector.h`.

The test suite `vectortest.c` and the header file `vector.h` were made readily available by the authors of the course material. I am only responsible for the implementation, i.e. the contents of the `vector.c` file.

