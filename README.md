Tesseract
=========

Tesseract is a library for performing diverse feature selection for linear regression problems using spectral regularizers.

Instruction
===========
0. install the dependencies (mentioned below)
1. create a directory named tests in the root directory
2. run "make" for compiling lib, unit/integration tests (goes inside tests dir) and documentation
3. run "make check" for running the tests

Dataset
=======
- MNIST dataset is obtained from [here](http://yann.lecun.com/exdb/mnist/)
- Boston Housing dataset is obtained from [here](https://archive.ics.uci.edu/ml/datasets/Housing)

Dependencies
============
- [Eigen3 linear algebra library](http://eigen.tuxfamily.org/index.php?title=Main_Page)
- GCC C++ compiler with C++11 features enabled (4.8.3 or higher)
- Doxygen
