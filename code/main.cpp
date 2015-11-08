// Main file for the lambda calculator.

#include <iostream>
#include <vector>
#include "element.hpp"
#include "token.hpp"
#include "tokenizer.hpp"

// Interprate the provided string, print it and print the result of an
// evaluation if possible, return the success of evaluation.
// Read, evaluate, print.
bool rep (char const * str)
{
  std::vector<char> chars;
  int i = 0;
  while ('\0' != str[i]) {
    chars.push_back(str[i]);
  }
  std::vector<Token> tokens(Tokenizer::mass(chars));
  i = 0;
  Element input(tokens, i);
  std::cout << "\"" << str << "\"" << std::endl;
  if (input.isExpretion())
  {
    Element * output = input.evaluate();
    output->print(std::cout);
    delete output;
    std::cout << std::endl;
    return true;
  }
  else
  {
    std::cout << "Input is not an expression." << std::endl;
    return false;
  }
}

int main (int argc, char * argv[])
{
  bool fin = true;
  for (int i = 1 ; i < argc ; ++i)
    fin = fin && rep(argv[i]);
  return (fin) ? 0 : 1;
  /*
  Element val("x.x");
  val.print(std::cout);
  std::cout << ':' << std::endl;
  std::cout << 'f' << val.isClosed() << std::endl;
  */
}
