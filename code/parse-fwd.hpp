#ifndef PARSE_FWD
#define PARSE_FWD

/* Defines the types used in the parsing of input.
 *
 * Some of them are almost certainly going to change so I decided to typedef
 * them and since I was making the file I made this a general typedef /
 * forward declaration file for the parsing part of the program.
 *
 * They are sort of like template arguments except global because they should
 * never have to have two seperate values in the same program. Appending a T
 * means they are a typedef of something else.
 *
 * Most of these types are used only as labels and so only copying, equality
 * and ordering needs (occationally) to be defined for them.
 */

// Type definition for the text type: reperesents the raw text that created
//   the token.
typedef char TextT;

// Type definition for the symbol type: reperesents the kind of token and
//   the symbols used in the parse tree.
enum class SymbolEnum : unsigned char;
typedef SymbolEnum SymbolT;

// Forwarded functions.
// Is a terminal symbol.
bool isTerminal (SymbolT);
// Is a token kind. Equivilant to isTerminal.
bool isTokenKind (SymbolT);
// Is a non-terminal symbol.
bool isNonTerminal (SymbolT);
// Is a symbol (either terminal or non-terminal).
bool isSymbol (SymbolT);
// Get the eof symbol.
SymbolT getEofSymbol ();

// The token type: full definition of the structure that reperents tokens.
class Token;

// Type definition for the state type:
typedef unsigned int StateT;

// Type definition for the parse tree type:
class ParseNode;

// Type definitions for the Production/Reduction Rule and Items there of:
class Rule;
class Item;

#endif//PARSE_FWD
