#ifndef TOKEN_HPP
#define TOKEN_HPP

/* The Token structure repersents a token from the tokenizer.
 * There are actually two restrictions on this type not enforced.
 * - kind should always be an isTokenKind value of SymbolT
 * - text should always be text that would generate a token of kind.
 * I decide enforcing it was not worth the trouble so... houner system.
 */

#include "parse-fwd.hpp"

struct Token
{
  // The type of the token.
  SymbolT kind;

  // For now all tokens are repersented by single characters.
  TextT text;
};

#endif//TOKEN_HPP