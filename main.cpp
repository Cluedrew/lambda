// Main file for the lambda calculator.

#include <iostream>
#include "element.hpp"

// Interprate the provided string, print it and print the result of an
// evaluation if possible, return the success of evaluation.
// Read, evaluate, print.
bool rep (char const * str)
{
  Element input(str);
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
  bool fin = true;// = rep("x.x");
  fin = fin && rep("(y.y z.z)");
  return (fin) ? 0 : 1;
  /*
  Element val("x.x");
  val.print(std::cout);
  std::cout << ':' << std::endl;
  std::cout << 'f' << val.isClosed() << std::endl;
  */
}
