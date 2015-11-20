#ifndef SYMBOL_HPP
#define SYMBOL_HPP 1

// The declaration of the symbol enumeration, used as SymbolT.

#include <iosfwd>

enum class SymbolEnum : unsigned char
{
  // Terminal / Token Symbols
  variable,
  dot,
  openApp,
  closeApp,
  // Token eof Symbol
  eof,
  // NonTerminal Symbols
  INPUT,
  PROGRAM,
  ELEMENT,
  FUNCTION,
  APPLICATION,
  // Count Limit
  cap
};

// Is a terminal symbol.
bool isTerminal (SymbolEnum sym);

// Is a token kind. Equivilant to isTerminal.
bool isTokenKind (SymbolEnum sym);

// Is a non-terminal symbol.
bool isNonTerminal (SymbolEnum sym);

// Is a symbol (either terminal or non-terminal).
// The only not symbol in the type is the cap, provided for looping.
bool isSymbol (SymbolEnum sym);

// Get the eof symbol.
SymbolEnum getEofSymbol ();

// Standard print operator overload.
std::ostream & operator<< (std::ostream & out, SymbolEnum sym);

// Standard read operator overload.
std::istream & operator>> (std::istream & in, SymbolEnum & sym);

#endif//SYMBOL_HPP
