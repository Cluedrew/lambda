#include "tokenizer.hpp"

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
      tokens.push_back(Token{Token::Dot, '.'});
      break;
    case '(':
      tokens.push_back(Token{Token::OpenApp, '('});
      break;
    case ')':
      tokens.push_back(Token{Token::CloseApp, ')'});
      break;
    default:
      if ('a' <= chs[i] && chs[i] <= 'z')
        tokens.push_back(Token{Token::Variable, chs[i]});
    }
  }
  return tokens;
}
