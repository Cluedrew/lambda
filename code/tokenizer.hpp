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
 *
 * Tokenizer works with istreams, use an istringstream wrapper if need be.
 */

#include <vector>
#include <string>
#include <istream>
class Token;

class Tokenizer
{
public:
  // static function preform 'instant' tokenization.

  static std::vector<Token> mass (std::istream &);
  /* Mass tokenization, transform the entire stream into tokens.
   * Params: A stream of input characters.
   * Return: A vector of tokens.
   */

  static Token single (char);
  /* Convert
   */

private:
  // Non-copyable
  Tokenizer (Tokenizer const &);
  Tokenizer & operator= (Tokenizer const &);

  // Input stream for each instance.
  std::istream & input;

protected:
public:
  Tokenizer (std::istream &);
  /* Create a Tokenizer that reads from the stream as it needs to.
   * Params: An input stream to read characters from.
   */

  virtual ~Tokenizer ();

  Token next();
  /* Get the next token in tokenization.
   * Effect: Reads from the stream far enough to find the next token.
   * Return: A new Token. If a the end of the stream has been reached than
   *   the eof Token is returned.
   */
};

#endif//TOKENIZER_HPP
