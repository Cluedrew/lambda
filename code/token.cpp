#include "token.hpp"

// The Token structure repersents a token from the tokenizer.

#include <iostream>
#include "symbol.hpp"



// The definition of the eofToken.
Token const eofToken = Token{getEofSymbol(), '\0'};

// Operator definitions.
bool operator== (Token t1, Token t2)
{
  return (t1.kind == t2.kind) && (t1.text == t2.text);
}

bool operator!= (Token t1, Token t2)
{
  return (t1.kind != t2.kind) || (t1.text != t2.text);
}

std::ostream & operator<< (std::ostream & out, Token const & token)
{
  return (out << token.kind << ":'" << token.text << '\'');
}
