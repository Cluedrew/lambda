#ifndef TOKEN_HPP
#define TOKEN_HPP

// The Token class repersents a token from the tokenizer.

struct Token
{
  enum TokenType
  {
    Parameter,
    Dot,
    OpenEval,
    CloseEval,

    Cap
  }
  type;

  //std::string text; For now all tokens are single characters.
  char text;
};

#endif//TOKEN_HPP
