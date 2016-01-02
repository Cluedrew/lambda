#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

/* A tokenizer class, scans through characters and produces tokens.
 *
 * A TokenStream does the same, except does it as needed and so will track its
 * progress. A TokenStream is non-copyable.
 *
 * I need to re-work this. This works but it is already outdated.
 * 1. Get rid of the TokenStream class, an instance of the Tokenizer will be
 *    the Token Stream. The current mass can stay ass a static function.
 * 2. A more easy way to write the logic for all tokenization would be nice,
 *    maybe something with std::regex (or my own with StateMachine).
 * 3. I can drop some of the old formats, I think std::istream and
 *    std::string should be enough input ways.
 */

#include <vector>
#include <string>
class Token;
class TokenStreamCore;

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

  static Token single (char);
  /* Convert
   */

  //static std::vector<Token> operator() (std::string);
  //static std::vector<Token> operator() (char const *);
};

class TokenStream
{
private:
  // Pointer to Implementation
  TokenStreamCore * heart;

  // Non-copyable
  TokenStream (TokenStream const &);
  TokenStream & operator= (TokenStream const &);
protected:
public:
  //TokenStream (Tokenizer const &, /*Some sort of character stream*/);

  TokenStream (std::vector<Token> const &);
  /* Create a TokenStream from a vector of tokens.
   * Params: A vector of Tokens containing all the tokens to be produced by
   *   the stream in order.
   */

  virtual ~TokenStream ();

  Token next ();
  /* Get the next Token from the stream. If all tokens have been read a
   *   special EOF token is returned instead.
   * Effect: Advances along the end of the stream.
   * Return: A new Token.
   */
};

#endif//TOKENIZER_HPP
