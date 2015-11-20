#include "tokenizer.hpp"

// Implementation of the Tokenizer and the TokenStream

#include <cstdlib>
#include "token.hpp"
#include "symbol.hpp"

// ===========================================================================
// Tokenizer =================================================================

// Mass tokenization, transform the entire vector into tokens.
std::vector<Token> Tokenizer::mass (std::vector<char> const & chs)
{
  std::vector<Token> tokens;
  unsigned int n = chs.size();
  for (unsigned int i = 0 ; i < n ; ++i)
  {
    switch (chs[i])
    {
    case '.':
      tokens.push_back(Token{SymbolT::dot, '.'});
      break;
    case '(':
      tokens.push_back(Token{SymbolT::openApp, '('});
      break;
    case ')':
      tokens.push_back(Token{SymbolT::closeApp, ')'});
      break;
    default:
      if ('a' <= chs[i] && chs[i] <= 'z')
        tokens.push_back(Token{SymbolT::variable, chs[i]});
    }
  }
  return tokens;
}

// ===========================================================================
/* TokenStreamCore ===========================================================
 * Pointer to Implementation, this class holds the secret data for TokenStream
 * as well as the situational implementations. All it really does is wrap up a
 * class hierarcy in TokenStream so I don't have to worry about it from the
 * outside.
 */

// The main class at the head of the hierarcy, abstract/pure virtual.
struct TokenStreamCore
{
  TokenStreamCore ();
  virtual ~TokenStreamCore () {}
  virtual Token next () =0;
  static Token const eofToken;
};

// A copy of the eof token for easy access and retrival.
Token const TokenStreamCore::eofToken = Token{getEofSymbol(), '\0'};

// Turns a vector into a stream.
class TokStrVectorWrapper
{
private:
  std::vector<Token> tokens;
  std::vector<Token>::iterator cur;
  std::vector<Token>::iterator end;
protected:
public:
  TokStrVectorWrapper (std::vector<Token> const & vec) :
      tokens(vec), cur(tokens.begin()), end(tokens.end())
  {}

  virtual ~TokStrVectorWrapper () {}

  Token next (){ return (cur == end) ? TokenStreamCore::eofToken : *(cur++); }
};

// ===========================================================================
// TokenStream ===============================================================

// Disabled Functions
TokenStream::TokenStream (TokenStream const & other)
{ exit(EXIT_FAILURE); }
TokenStream & TokenStream::operator= (TokenStream const & other)
{ exit(EXIT_FAILURE); return *this; }

// Constrctors and Deconstructor =============================================
//TokenStream::TokenStream (Tokenizer const & tokenizer,
//                          /*Some sort of character stream*/) :
//  heart(...)
//{}

// Create a TokenStream from a vector of tokens.
TokenStream::TokenStream (std::vector<Token> const & vec) :
  heart((TokenStreamCore*)new TokStrVectorWrapper(vec))
{}

// Deconstructor:
TokenStream::~TokenStream ()
{ delete heart; }

// Get the next Token from the stream.
Token TokenStream::next ()
{ return heart->next(); }
