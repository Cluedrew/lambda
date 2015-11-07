#include "element.hpp"

// Implemention of the Element class.

#include <ostream>
#include "substution-op.hpp"

// Constructors and Deconstructor ============================================
// Create an element from a string.
Element::Element (char const * str) :
  core('\0'), lhs(nullptr), rhs(nullptr)
{
  int pos = 0;
  construct(str, pos);
}

// Create an element from part of a string.
Element::Element (char const * str, int & pos) :
  core('\0'), lhs(nullptr), rhs(nullptr)
{
  construct(str, pos);
}

// Advance to the next non-(white)space character.
void Advance (char const * str, int & pos)
{ ++pos; while (' ' == str[pos]) ++pos; }

// Helpper used by the above constructors.
void Element::construct(char const * str, int & pos)
{
  // Advance to a non-space character.
  //while (' ' == str[pos]) ++pos;

  // Create an evaluation.
  if ('(' == str[pos])
  {
    core = '(';
    //Advance(str, pos);
    pos++;
    lhs = new Element(str, pos);
    pos++;
    rhs = new Element(str, pos);
    pos++;
    // skip the ')'
    //Advance(str, pos);
  }
  // Create a function.
  else if ('.' == str[pos + 1])
  {
    core = '.';
    lhs = new Element(str[pos]);
    //Advance(str, pos);
    pos++; pos++;
    rhs = new Element(str, pos);
  }
  // Create a parameter.
  else
  {
    core = str[pos];
    pos++;
    //Advance(str, pos);
  }
  // std::cerr << '(' << core << ',' << lhs << ',' << rhs << ')' << std::endl;
}

// Parameter constructor, short cut to the given parameter.
Element::Element (char letter) :
  core(letter), lhs(nullptr), rhs(nullptr)
{}

// By field constructor, for internal use only.
Element::Element (char symbol, Element * left, Element * right) :
  core(symbol), lhs(left), rhs(right)
{}

// Copy constructor (Deep Copy)
Element::Element (Element const & other) :
  core(other.core),
  lhs(other.lhs ? new Element(*other.lhs) : nullptr),
  rhs(other.rhs ? new Element(*other.rhs) : nullptr)
{}

// Deconstructor
Element::~Element ()
{
  delete lhs;
  delete rhs;
}

// Predicate Functions =======================================================
// Check to see if the element is of the given type.
bool Element::isFunction () const
{ return '.' == core; }

// Check to see if the element is of the given type.
bool Element::isEvaluation () const
{ return '(' == core; }

// Check to see if the element is of the given type.
bool Element::isParameter () const
{ return 'a' <= core && core <= 'z'; }

// Check to see if the element is closed.
bool Element::isClosed () const
{
  if ('.' == core)
    // If everything in the right is bound to the parameter on the left,
    // or some later function header, the function is closed.
    return rhs->isClosedWith(std::string(1, lhs->core));
  else if ('(' == core)
    // Evaluations should check the right and left hand sides.
    return lhs->isClosed() && rhs->isClosed();
  else
    // Parameters are not closed without context.
    return false;
}

// Check to see if the element is closed within a given context.
bool Element::isClosedWith (std::string bound) const
{
  // Functions bind their parameter and check to see if everything in their
  // body is bound.
  if ('.' == core)
    if (bound.find(lhs->core))
      return rhs->isClosedWith(bound);
    else
      return rhs->isClosedWith(bound + lhs->core);
  // Evaluations are closed if there left and right sides are closed.
  else if ('(' == core)
    return lhs->isClosedWith(bound) && rhs->isClosedWith(bound);
  // A parameter is closed if it appears in the list of bound parameters.
  else
    return std::string::npos != bound.find(core);
}

// Check to see if the element is an expretion.
bool Element::isExpretion () const
{
  // Evaluations are exprestions if there left and right sides are.
  if ('(' == core)
    return lhs->isExpretion() && rhs->isExpretion();
  // Functions are exprestions if they are closed.
  else if ('.' == core)
    return isClosed();
  // Parameters are not exprestions.
  else
    return false;
}

// Operation Functions =======================================================

// There are some memory issues that need to be dealt with.

// Get the result of an evaluation on the exprestion.
Element * Element::evaluate () const
{
  // if (!isExpretion()) error!
  if ('(' == core)
  {
    // return lhs->evaluate().apply(rhs->evaluate()); with memory protection.
    Element * tmpl = lhs->evaluate();
    Element * tmpr = rhs->evaluate();
    Element * tmpf = tmpl->apply(tmpr);
    delete tmpl;
    delete tmpr;
    return tmpf;
  }
  else // ('.' == core)
    return new Element(*this);
}

// Get the result of an application on a function.
Element * Element::apply (Element * value) const
{
  // if (! isFunction()) error!
  SubstutionOp subOp = {lhs->core, value};
  return rhs->substute(subOp);
}

// Get the result of a substution on an element.
Element * Element::substute (SubstutionOp & subOp) const
{
  // Function
  if ('.' == core)
    return (subOp.replaced == lhs->core) ? new Element(*this) :
        new Element('.', new Element(lhs->core), rhs->substute(subOp));
  // Evaluation
  else if ('(' == core)
    return new Element('(', lhs->substute(subOp), rhs->substute(subOp));
  // Parameter
  else
    return (subOp.replaced == core) ?
        new Element(*subOp.replaces) :
        new Element(core);
}

// Output Functions ==========================================================
// Print the element to the given stream.
void Element::print (std::ostream & out)
{
  if ('.' == core)
  {
    lhs->print(out);
    out.put('.');
    rhs->print(out);
  }
  else if ('(' == core)
  {
    out.put('(');
    lhs->print(out);
    out.put(' ');
    rhs->print(out);
    out.put(')');
  }
  else
  {
    out.put(core);
  }
  out << std::flush;
}
