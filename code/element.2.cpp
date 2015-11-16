#include "element.hpp"

// WIP Remember the _2_s are going to be removed.

// Implemention of the Element class.

#include <ostream>
#include "parse-node.hpp"
#include "substution-op.hpp"
#include "token.hpp"
#include "symbol.hpp"

// Constructors and Deconstructor ============================================
// New Constructor ===========================================================
// Construct an element from a ParseNode.
Element::Element (ParseNode * node)
{
  if (!node->getHead() == SymbolEnum::ELEMENT)
  {
    // error: Might try and add more information.
    throw std::invalid_argument("Element constructor recived bad node");
  }

  // Currently, to checking the first symbol is enough know exactly what
  //   Rule is being used.
  switch (node->getISymbol(0))
  {
  case SymbolEnum::FUNCTION:
    // ELEMENT FUNCTION / FUNCTION variable dot ELEMENT
    type = function;
    ParseNode const & inner = node->child(0);
    fun.head.id = inner.child(0).getText();
    fun.body = new Element(&inner.child(2));
    break;
  case SymbolEnum::APPLICATION:
    // ELEMENT APPLICATION / APPLICATION openApp ELEMENT ELEMENT closeApp
    type = application;
    ParseNode const & inner = node->child(0);
    app.lhs = new Element(&inner.child(1));
    app.rhs = new Element(&inner.child(2));
    break;
  case SymbolEnum::variable:
    // ELEMENT variable
    type = variable;
    var.id = node->child(0).getText();
    break;
  default:
    // FAULT! (false just means there was a BIG problem)
    throw false;
  };
}

// Copy constructor (Deep Copy)
Element::Element (Element const & other) :
  type(other.type)
{
  switch (type)
  {
  case function:
    fun.head.id = other.fun.head.id;
    fun.body = new Element(other.fun.body);
    break;
  case application:
    app.lhs = new Element(other.app.lhs);
    app.rhs = new Element(other.app.rhs);
    break;
  default:
    var.id = other.var.id;
    break;
  }
}

// Deconstructor
Element::~Element ()
{
  switch (type)
  {
  case function:
    delete fun.body;
    break;
  case application:
    delete app.lhs;
    delete app.rhs;
    break;
  default:
    break;
  }
}

// Predicate Functions =======================================================
// Check to see if the element is of the given type.
bool Element::isFunction () const
{ return ElementType::function == type; }

// Check to see if the element is of the given type.
bool Element::isApplication () const
{ return ElementType::application == type; }

// Check to see if the element is of the given type.
bool Element::isVariable () const
{ return ElementType::variable == type; }

// Check to see if the element is closed.
bool Element::isClosed () const
{
  if (isFunction())
    // If everything in the right is bound to the parameter on the left,
    // or some later function header, the function is closed.
    return rhs->isClosedWith(std::string(1, lhs->core));
  else if (isApplication())
    // Evaluations should check the right and left hand sides.
    return lhs->isClosed() && rhs->isClosed();
  else
    // Parameters are not closed without context.
    return false;
}

// Check to see if the element is closed within a given context.
bool Element::isClosedWith (std::vector<Variable *> bounded) const
{}

// Check to see if the element is an expretion.
bool Element::isExpretion () const
{}

// Operation Functions =======================================================

// Get the result of an evaluation on the exprestion.
Element * Element::evaluate () const
{}

// Get the result of an application on a function.
Element * Element::apply (Element * value) const
{}

// Get the result of a substution on an element.
Element * Element::substute (SubstutionOp & subOp) const
{}

// Output Functions ==========================================================
// Print the element to the given stream.
void Element::print (std::ostream & out)
{}
