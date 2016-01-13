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
#include "cfg-parser.hpp"
#include "parse-node.hpp"

// Given a file name, return a CFG read from it.
CFGrammer readCFGFromFile (char const * fileName)
{
  std::cout << "Reading Grammer from " << fileName << std::endl;

  std::ifstream defFile(fileName);
  CFGrammer cfg = CFGrammer::defineFromText(defFile);
  defFile.close();

  return cfg;
}

// Given a CFG, attempt to generate an action table from it with SLR1.
ActionTable makeSlr1ActionTable (CFGrammer const & cfg)
{
  std::cout << "Perparing ActionTableGenerator" << std::endl;
  Slr1Atg atg(cfg);

  // If all is good return the action table.
  if (atg.canGenerate())
  {
    std::cout << "Perparing ActionTable" << std::endl;
    return atg.generate();
  }
  // On error print the error information and return an empty table.
  else
  {
    std::cout << "Invalid Generator" << std::endl;
    atg.printProblems(std::cout);
    return ActionTable();
  }
}

// Giving a file name, read in an Action Table from it.
ActionTable readActionTableFromFile (char const * fileName)
{
  std::cout << "Reading Action Table from " << fileName << std::endl;

  std::ifstream defFile(fileName);
  ActionTable at;
  defFile >> at;
  defFile.close();

  return at;
}



int main (int argc, char * argv[])
{
  // Get the parser.
  CFGParser parser(readCFGFromFile("lang.cfg"),
                   readActionTableFromFile("lang.sr"));

  // Prepare a Tokenizer
  Tokenizer tokenStream(std::cin);

  // Parse the file.
  ParseNode * parseRootNode = parser.parse(tokenStream);

  // ParseTree->CodeTree
  LambdaElement * codeRoot = LambdaElement::fromParseRoot(parseRootNode);

  // Free the parse tree.
  delete parseRootNode;
  parseRootNode = NULL;

  // Print the code tree.
  codeRoot->write(std::cout);
  std::cout << std::endl;

  // Free the code tree.
  delete codeRoot;
  codeRoot = NULL;

  return 0;
}
