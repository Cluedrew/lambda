#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

/* A tokenizer class, scans through characters and produces tokens.
 */

#include <vector>
#include <string>
#include "token.hpp"

class Tokenizer
{
private:
protected:
public:

  static std::vector<Token> operator() (std::vector<char>);
  /* Mass tokenization, transform the entire vector into tokens.
   */

  //static std::vector<Token> operator() (std::string);
  //static std::vector<Token> operator() (char const *);
};

#endif//TOKENIZER_HPP
