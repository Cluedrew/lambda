#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

/* A tokenizer class, scans through characters and produces tokens.
 *
 * A TokenSteam does the same, except does it as needed and so also keeps
 * track of its currant location.
 */

#include <vector>
#include <string>
#include "token.hpp"

class Tokenizer
{
private:
protected:
public:

  static std::vector<Token> mass (std::vector<char> const &);
  /* Mass tokenization, transform the entire vector into tokens.
   * Params: A vector of input characters.
   * Return: A vector of tokens.
   */

  //static std::vector<Token> operator() (std::string);
  //static std::vector<Token> operator() (char const *);
};

class TokenStream
{
private:
protected:
public:
  TokenStream (Tokenizer const &, /*Some sort of character stream*/);

  virtual ~TokenStream ();

  Token next ();
  /* Get the next Token from the stream. If all tokens have been read a
   *   special EOF token is returned instead.
   * Effect: Advances along the end of the stream.
   * Return: A new Token.
   */
};

#endif//TOKENIZER_HPP
