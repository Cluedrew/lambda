#ifndef TOKEN_HPP
#define TOKEN_HPP

// The Token class repersents a token from the tokenizer.

struct Token
{
  enum TokenType
  {
    Variable,
    Dot,
    OpenApp,
    CloseApp,

    Cap
  };

  // The type of the token.
  TokenType type;

  // For now all tokens are repersented by single characters.
  char text;
};

#endif//TOKEN_HPP
