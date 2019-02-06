#ifndef __SHAREDVARS_H
#define __SHAREDVARS_H

#include <vector>

// Default string length for character arrays.
const int defaultStringLength = 255;

// Tokens are delimited by the = sign.
const char tokenSplitter = '=';

// Define a string (char *) vector and its iterator.
typedef std::vector<char*> charstarVector;
typedef charstarVector::iterator charstarVectorIt;

// Define a boolean vector and its iterator.
typedef std::vector<bool> boolVector;
typedef boolVector::iterator boolVectorIt;

#endif
