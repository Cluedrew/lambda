// Main file for the lambda calculator.

#include <iostream>
#include <fstream>
#include <vector>
#include "element.hpp"
#include "cfgrammer.hpp"
#include "token.hpp"
#include "tokenizer.hpp"

int main (int argc, char * argv[])
{
  CFGrammer clg = CFGrammer::defineFromText( ... );
  return 0;
}
