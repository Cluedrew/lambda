#include "tokenizer.hpp"

// Implementation of the Tokenizer and the TokenStream

#include <cstdlib>
#include <iostream>
#include "token.hpp"
#include "symbol.hpp"



// ===========================================================================
// Tokenizer =================================================================

// Create a Tokenizer that reads from the stream as it needs to.
Tokenizer::Tokenizer (std::istream & in) :
  input(in)
{}

// Deconstructor
Tokenizer::~Tokenizer ()
{}



// Get the next token in tokenization.
Token Tokenizer::next()
{
  // If we are already reached the end of output return the eof.
  if (!input.eof())
  {
    // Search forward for the next token.
    do {
      // Get the next character.
      int ch = input.get();

      std::cerr << ch << std::endl;

      // Check to see if it forms a token.
      if (ch == '.')
        return Token{SymbolT::dot, '.'};
      else if (ch == '(')
        return Token{SymbolT::openApp, '('};
      else if (ch == ')')
        return Token{SymbolT::closeApp, ')'};
      else if ('a' <= ch && ch <= 'z')
        return Token{SymbolT::variable, char(ch)};

    } while (!input.eof());
  }

  // If we reached the end of input produce the eofToken.
  return eofToken;
}

// Mass tokenization, transform the entire stream into tokens.
std::vector<Token> Tokenizer::mass (std::istream & in)
{
  std::vector<Token> tokens;
  Tokenizer internal(in);
  Token nextToken;

  while (eofToken != (nextToken = internal.next()) )
  {
    tokens.push_back(nextToken);
  }

  return tokens;
}



// Disabled Functions
Tokenizer::Tokenizer (Tokenizer const & other) :
  input(other.input)
{
  exit(EXIT_FAILURE);
}
Tokenizer & Tokenizer::operator= (Tokenizer const & other)
{
  exit(EXIT_FAILURE);
  return *this;
}
