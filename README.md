# Break-Even Volatility

This repository holds a small codebase for calculating the break-even volatility (henceforth, BEV) of a financial security based on a single historical time series of asset prices. In essense, BEV is a method of calculating historical volatility, but differs from the usual annualised standard deviation of log returns which ignores any market skew/variation in pricing, if evident. A single BEV estimate can be found for a particular strike-price/maturity combination, or an entire volatility skew for a range of strikes, or additionally with a range of terms/option maturities, one can obtain a full volatility surface.

For more information on the break-even volatility, please see [this review](https://open.uct.ac.za/handle/11427/30980) of the methodology. The methodology currently implemented does not involve discrete delta-hedging, but rather the discrete approximation of the continuously delta-hedged profit/loss function, named the "Traditional BEV formula" in section 2.2.

## Library summary

In the root directory, we have [main.cpp](main.cpp) and a data file [GOOG.csv](GOOG.csv) holding five years of Alphabet Inc.'s (GOOG) share price history downloaded from [Yahoo Finance](https://finance.yahoo.com/quote/GOOG/history?p=GOOG). [main.cpp](main.cpp) forms the first minimum working example, with more available in the [examples](examples) directory. One can substitute their own data into the root directory and modify [main.cpp](main.cpp) accordingly.

In the [bev](bev) directory, we have the BEV class declaration and definition in [bev.h](bev/bev.h) and [bev.cpp](bev/bev.cpp), respectively. This class allows users to run the break-even volatility method on data inputted from the CSV's filepath or from an [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page) array, on a range of specified strikes and maturities.

Supporting the BEV class is a number of utility functions found in the [utils](bev/utils) subdirectory. The functions' declarations and implementations have also been separated, except for the cumsum template functions and NormPDF function which operate on Eigen arrays or matrices. Other functions include data processing and simulation, root finding algorithms for the BEV calculation and a function with some nicer formatting for command-line output of BEV results.

## Getting started

### Requirements

This codebase was built and tested using GCC (g++) with the C++14 standard. For building with CMake, the project was built using a recent version (3.26.4), but an earlier version will most likely suffice.

User's will also need to have downloaded the [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page) library. Eigen is a template library for linear algebra and vector, matrix and array operations (amongst others). See documentation [here](https://eigen.tuxfamily.org/dox/). This codebase was built and tested with version 3.4.0.

### Building 

#### With CMake

If building with CMake, the only modifications the user may have to make is, firstly, to the minimum CMake version required if necessary, and secondly, to update the PATH_TO_EIGEN_LIB variable to the location of the user's Eigen library. Both of these changes occur in the top-level [CMakeLists.txt](CMakeLists.txt) file.

To build, create a "build" directory and CMake does the rest. For example, from the root directory:
```
mkdir build
cd build
cmake ..
cmake --build .
```
The executables will be written to their respective source's directory, not the build directory, i.e. root for main.cpp and in the [examples](examples) directory.

Note: if GCC/g++ was obtained through MinGW then an additional flag may be needed on the first call to cmake as such:
```
cmake .. -G "MinGW Makefiles"
```

#### Without CMake

Since the source files do not use/include relative paths to other header files, one must then include the paths to each header file needed when compiling separately, or when compiling and linking all libraries and sub-libraries at once. Thus, the easiest solution may be to take all header and source files and group them in one/the root directory. This saves the need for multiple include flags when compiling. However, one include flag will be needed and that is the path to the user's Eigen library. For example, with g++, command-line compilation with all files in one directory would look like:
```
g++ -I path/to/eigen -c utils.cpp
g++ -I path/to/eigen -c bev.cpp
g++ -I path/to/eigen -o out main.cpp bev.o utils.o
```
Or in one shot:
```
g++ -I path/to/eigen -o out main.cpp bev.cpp utils.cpp
```
Keeping the repository's structure as is, the previous line would rather look like:
```
g++ -I path/to/eigen -I bev -I bev/utils -o out main.cpp bev/bev.cpp bev/utils/utils.cpp
```
The former, multiline case would change similarly if the bev and utils object files were to be created separately.
