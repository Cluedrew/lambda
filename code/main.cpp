// Main file for the lambda calculator.

#include <iostream>
#include <fstream>
#include <vector>
#include "element.hpp"
#include "cfgrammer.hpp"
#include "token.hpp"
#include "tokenizer.hpp"
#include "slr1-atg.hpp"
#include "action-table.hpp"

int main (int argc, char * argv[])
{
  std::cout << "Perparing Grammer" << std::endl;
  std::ifstream defFile("lang.cfg");
  CFGrammer cfg = CFGrammer::defineFromText(defFile);
  defFile.close();
  std::cout << cfg;

  std::cout << "Perparing ActionTableGenerator" << std::endl;
  Slr1Atg atg(cfg);

  if (atg.canGenerate())
  {
    std::cout << "Perparing ActionTable" << std::endl;
    ActionTable at = atg.generate();
    std::cout << at;
    return 0;
  }
  else
    return 1;
}
