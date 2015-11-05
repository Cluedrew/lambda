#include "element.hpp"

// Constructors and Deconstructor ============================================
Element::Element (char const * str) :
  core('\0'), lhs(nullptr), rhs(nullptr)
{
  int pos = 0;
  construct(str, pos);
}

Element::Element (char const * str, int & pos) :
  core('\0'), lhs(nullptr), rhs(nullptr)
{
  construct(str, pos);
}

// Helpper used by the above constructors.
void Element::construct(char const * str, int & pos)
{
  // Create an evaluation.
  if ('(' == str[pos])
  {
  }
  // Create a function.
  else if ('.' == str[pos + 1])
  {
  }
  // Create a parameter.
  else
  {
  }
}

Element::Element (char letter) :
  core(letter), lhs(nullptr), rhs(nullptr)
{}

// Copy constructor
Element::Element (Element const & other) :
  core(other.core),
  lhs(other.lhs ? new Element(other.lhs) : nullptr),
  rhs(other.rhs ? new Element(other.rhs) : nullptr)
{}

// Deconstructor
Element::~Element ()
{
  delete lhs;
  delete rhs;
}

bool Element::isFunction ()
{ return '.' == core; }

bool Element::isEvaluation ()
{ return '(' == core; }

bool Element::isParameter ()
{ return 'a' <= core && core <= 'z'; }

bool Element::isClosed ();
bool Element::isClosedWith (std::string);

bool Element::isExpretion ();

Element * Element::evaluate ();
Element * Element::apply (Element *);
Element * Element::substute (SubstutionOp &);

