#ifndef TOKEN_HPP
#define TOKEN_HPP

// The Token class repersents a token from the tokenizer.

struct Token
{
  enum TokenType
  {
    variable,
    dot,
    openApp,
    closeApp,

    eof
  };

  // The type of the token.
  TokenType type;

  // For now all tokens are repersented by single characters.
  char text;

  // The eof token is a special placeholder.
  static Token newEOF () { return Token{eof, '\0'}; }
};

#endif//TOKEN_HPP
