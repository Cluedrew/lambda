#include "tokenizer.hpp"

// Mass tokenization, transform the entire vector into tokens.
static std::vector<Token> operator() (std::vector<char> chs)
{
  unsigned int n = chs.size();
  for (unsigned int i = 0 ; i < n ; ++i)
  {
    switch (chs[n])
    {
    case '.':
      chs.push_back(Token{Token::Dot, '.'});
      break;
    case '(':
      chs.push_back(Token{Token::OpenEval, '('});
      break;
    case ')':
      chs.push_back(Token{Token::CloseEval, ')'});
      break;
    default:
      if ('a' <= chs[n] && chs[n] <= 'z')
        chs.push_back(Token{Token::Parameter, chs[n]});
    }
  }
}
