#ifndef PARSE_FWD
#define PARSE_FWD

/* Defines the types used in the parsing of input.
 *
 * Some of them are almost certainly going to change so I decided to typedef
 * them and since I was making the file I made this a general typedef /
 * forward declaration file for the parsing part of the program.
 *
 * They are sort of like template arguments except global because they should
 * never have to have two seperate values in the same program.
 *
 * Most of these types are used only as labels and so only copying, equality
 * and ordering needs (occationally) to be defined for them.
 */

// Type definition for the text type: reperesents the raw text that created
//   the token.
typedef char TextT;

// Type definition for the symbol type: reperesents the kind of token and
//   the symbols used in the parse tree.
typedef SymbolEnum SymbolT;
/*
bool isTerminal (SymbolT); // Is a terminal symbol.
bool isTokenKind (SymbolT); // Equivilant to isTerminal.
bool isNonTerminal (SymbolT); // Is a non-terminal symbol.
bool isSymbol (SymbolT); // Is a symbol (either terminal or non-terminal).
*/

// The token type: full definition of the structure that reperents tokens.
#include "token.hpp"
// struct Token { SymbolT kind; TextT text; };

// Type definition for the state type:
typedef unsigned int StateT;

// Type definition for the parse tree type:
#include "parse-node.hpp"
// class ParseNode; typedef ParseTreeT ParseNode;

#endif//PARSE_FWD
