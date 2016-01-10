#ifndef TOKEN_HPP
#define TOKEN_HPP

/* The Token structure repersents a token from the tokenizer.
 * There are actually two restrictions on this type not enforced.
 * - kind should always be an isTokenKind value of SymbolT
 * - text should always be text that would generate a token of kind.
 * I decide enforcing it was not worth the trouble so... houner system.
 */

#include <iosfwd>
#include "parse-fwd.hpp"

struct Token
{
  // The type of the token.
  SymbolT kind;

  // For now all tokens are repersented by single characters.
  TextT text;
};



// Definition of the end of stream marker.
extern Token const eofToken;



// Operator definitions.
bool operator== (Token t1, Token t2);

bool operator!= (Token t1, Token t2);

std::ostream & operator<< (std::ostream & out, Token const & token);

#endif//TOKEN_HPP
