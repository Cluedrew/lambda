#ifndef SYMBOL_HPP
#define SYMBOL_HPP 1

// The declaration of the symbol enumeration, used as SymbolT.

enum class SymbolEnum : unsigned char
{
  // Terminal / Token Symbols
  // Token eof Symbol
  eof,
  // NonTerminal Symbols
  // Count Limit
  cap
};

// Is a terminal symbol.
inline bool isTerminal (SymbolEnum sym)
{ return sym <= eof; }

// Is a token kind. Equivilant to isTerminal.
inline bool isTokenKind (SymbolEnum sym)
{ return sym <= eof; }

// Is a non-terminal symbol.
inline bool isNonTerminal (SymbolEnum sym)
{ return eof < sym && sym < cap; }

// Is a symbol (either terminal or non-terminal).
// The only not symbol in the type is the cap, provided for looping.
inline bool isSymbol (SymbolEnum sym)
{ return sym < cap; }

#endif//SYMBOL_HPP
