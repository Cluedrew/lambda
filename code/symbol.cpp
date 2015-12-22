#include "symbol.hpp"

// Implementation of the SymbolEnum functions.
// All 4 of the s*.hpp file had no .cpp partner before this.

#include <istream>
#include <ostream>
#include <string>
#include <stdexcept>

// Predicate Functions =======================================================

// Is a terminal symbol.
bool isTerminal (SymbolEnum sym)
{ return sym <= SymbolEnum::eof; }

// Is a token kind. Equivilant to isTerminal.
bool isTokenKind (SymbolEnum sym)
{ return sym <= SymbolEnum::eof; }

// Is a non-terminal symbol.
bool isNonTerminal (SymbolEnum sym)
{ return SymbolEnum::eof < sym && sym < SymbolEnum::cap; }

// Is a symbol (either terminal or non-terminal).
// The only not symbol in the type is the cap, provided for looping.
bool isSymbol (SymbolEnum sym)
{ return sym < SymbolEnum::cap; }

// Getters ===================================================================
// Get the eof symbol.
SymbolEnum getEofSymbol ()
{ return SymbolEnum::eof; }

// Type Conversion Functions =================================================
// Convert a symbol into a string.
std::string symbolToString (SymbolEnum symbol)
{
  switch (symbol)
  {
#define SYMTOSTR(name) case SymbolEnum::name: return # name ;

  SYMTOSTR(variable)
  SYMTOSTR(dot)
  SYMTOSTR(openApp)
  SYMTOSTR(closeApp)
  SYMTOSTR(eof)
  SYMTOSTR(INPUT)
  SYMTOSTR(PROGRAM)
  SYMTOSTR(ELEMENT)
  SYMTOSTR(FUNCTION)
  SYMTOSTR(APPLICATION)

#undef SYMTOSTR

  default: // cap
    throw std::invalid_argument("symbolToString: invalid argument");
    return "";
  }
}

// Convert a string into a symbol.
SymbolEnum stringToSymbol (std::string string)
{
#define STRTOSYM(name) \
    if (std::string( # name ) == string) \
      return SymbolEnum::name;

  /**/ STRTOSYM(variable)
  else STRTOSYM(dot)
  else STRTOSYM(openApp)
  else STRTOSYM(closeApp)
  else STRTOSYM(eof)
  else STRTOSYM(INPUT)
  else STRTOSYM(PROGRAM)
  else STRTOSYM(ELEMENT)
  else STRTOSYM(FUNCTION)
  else STRTOSYM(APPLICATION)

#undef STRTOSYM

  else
    throw std::invalid_argument("symbolToString: invalid argument");
}

// In/Output Operators =======================================================

// Standard print operator overload.
std::ostream & operator<< (std::ostream & out, SymbolEnum sym)
{
  // Just a really crude mapping.
  switch (sym)
  {
#define SYMTOSTR(name) case SymbolEnum::name: return out << # name ; break;

  SYMTOSTR(variable)
  SYMTOSTR(dot)
  SYMTOSTR(openApp)
  SYMTOSTR(closeApp)
  SYMTOSTR(eof)
  SYMTOSTR(INPUT)
  SYMTOSTR(PROGRAM)
  SYMTOSTR(ELEMENT)
  SYMTOSTR(FUNCTION)
  SYMTOSTR(APPLICATION)

#undef SYMTOSTR

  default: // cap
    throw std::invalid_argument("SymbolEnum::operator<< invalid argument");
    return out;
  }
}

// Standard read operator overload.
std::istream & operator>> (std::istream & in, SymbolEnum & sym)
{
  std::string input;
  in >> input;

#define STRTOSYM(name) \
    if (std::string( # name ) == input) \
      sym = SymbolEnum::name;

  /**/ STRTOSYM(variable)
  else STRTOSYM(dot)
  else STRTOSYM(openApp)
  else STRTOSYM(closeApp)
  else STRTOSYM(eof)
  else STRTOSYM(INPUT)
  else STRTOSYM(PROGRAM)
  else STRTOSYM(ELEMENT)
  else STRTOSYM(FUNCTION)
  else STRTOSYM(APPLICATION)

#undef STRTOSYM

  else in.setstate(std::ios_base::badbit);

  return in;
}

// Pre/Post-increment operators ==============================================
SymbolEnum & operator++ (SymbolEnum & sym)
{
#define SYM_FROM_TO(f,t) case SymbolEnum::f: return sym = SymbolEnum::t;
  switch (sym)
  {
  SYM_FROM_TO(variable,dot)
  SYM_FROM_TO(dot,openApp)
  SYM_FROM_TO(openApp,closeApp)
  SYM_FROM_TO(closeApp,eof)
  SYM_FROM_TO(eof,INPUT)
  SYM_FROM_TO(INPUT,PROGRAM)
  SYM_FROM_TO(PROGRAM,ELEMENT)
  SYM_FROM_TO(ELEMENT,FUNCTION)
  SYM_FROM_TO(FUNCTION,APPLICATION)
  SYM_FROM_TO(APPLICATION,cap)
  default:
    return sym = SymbolEnum::cap;
  }
#undef SYM_FROM_TO
}

SymbolEnum operator++ (SymbolEnum & sym, int)
{
  SymbolEnum tmp = sym;
  operator++(sym);
  return tmp;
}
