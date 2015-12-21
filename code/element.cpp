#include "element.hpp"

// Implemention of the Element class.

#include <ostream>
#include "parse-node.hpp"
#include "substution-op.hpp"
#include "token.hpp"
#include "symbol.hpp"

/*
  switch (type)
  {
  case function:
  case application:
  case variable:
  }
*/

// Constructors and Deconstructor ============================================
// Construct an element from a ParseNode.
Element::Element (ParseNode const * node)
{
  if (node->getHead() != SymbolEnum::ELEMENT)
  {
    // error: Might try and add more information.
    throw std::invalid_argument("Element constructor recived bad node");
  }

  // Currently, to checking the first symbol is enough know exactly what
  //   Rule is being used.
  switch (inner->getHead(0))
  {
  case SymbolEnum::FUNCTION:
    // ELEMENT FUNCTION / FUNCTION variable dot ELEMENT
    type = function;
    ParseNode const * inner = node->child(0);
    fun.head.id = inner->child(0)->getText();
    fun.body = new Element(inner->child(2));
    break;
  case SymbolEnum::APPLICATION:
    // ELEMENT APPLICATION / APPLICATION openApp ELEMENT ELEMENT closeApp
    type = application;
    ParseNode const * inner = node->child(0);
    app.lhs = new Element(inner->child(1));
    app.rhs = new Element(inner->child(2));
    break;
  case SymbolEnum::variable:
    // ELEMENT variable
    type = variable;
    var.id = node->child(0)->getText();
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

// Create a variable element.
Element::Element (TextT text) :
  type(variable), var.id(text)
{}

// Create a function element.
Element::Element (TextT head, Element * body) :
  type(function), fun.head.id(head), fun(body)
{}

// Create an application element.
Element::Element (Element * lhs, Element * rhs) :
  type(application), app.lhs(lhs), app.rhs(rhs)
{}

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
  switch (type)
  {
  // If everything in the right is bound to the parameter on the left,
  // or some later function header, the function is closed.
  case function:
    return fun.body->isClosedWith(std::vector(1, &fun.head));
  // Evaluations should check the right and left hand sides.
  case application:
    return app.lhs->isClosed() && app.rhs->isClosed();
  // Parameters are not closed without context.
  case variable:
    return false;
  }
}

// Check to see if the element is closed within a given context.
bool Element::isClosedWith (std::vector<Variable *> bounded) const
{
  switch (type)
  {
  // Functions change the context before passing it down.
  case function:
    for (unsigned int i = 0 ; i < bounded.size() ; ++i)
      if (bounded[i]->id == fun.head.id)
        return fun.body->isClosedWith( (bounded[i] = &fun.head) );
    bounded.push_back(&fun.head);
    return fun.body->isClosedWith(bounded);
  // Applications just pass down to the two sides.
  case application:
    return (app.lhs->isClosedWith(bounded) && app.rhs->isClosedWith(bound));
  // Variables finally check to see if they are bounded.
  case variable:
    for (unsigned int i = 0 ; i < bounded.size() ; ++i)
      if (bounded[i]->id == var.id)
        return true;
    return false;
  }
}
//class Context
//{
//private:
//  std::vector<Variable *> varBinds;
//public:
//  Context ();
//  Context (Variable ~);
//  Context (Context const *);
//  ~Context ();
//  void introduce (Variable *);
//  void introduce (Variable &);
//  bool isBound (Variable ~) const;
//  Variable * getBind (Variable ~);
//}

// Check to see if the element is an expression.
bool Element::isExpression () const
{
  switch (type)
  {
  // Functions are expressions if they are closed.
  case function:
    return isClosed());
  // Applications are expressions if their two sides are expressions.
  case application:
    return app.lhs->isExpression() && app.rhs->isExpression();
  // Variables are not expressions.
  case variable:
    return false;
  }
}

// Operation Functions =======================================================

// Get the result of an evaluation on the exprestion.
Element * Element::evaluate () const
{
  if (!isExpression())
    throw ...;

  // Closed Functions are there own result.
  if (isFunction())
    return new Element(*this);
  // Applications apply the rhs to the lhs.
  else
  {
    Element * lhsResult = evaluate(*app.lhs);
    Element * rhsResult = evaluate(*app.rhs);
    Element * finResult = lhsResult->apply(&rhsResult);
    delete lhsResult;
    delete rhsResult;
    return finResult;
  }
}

// Get the result of an application on a function.
Element * Element::apply (Element const & value) const
{
  if (isFunction())
  {
    SubstutionOp op;
    op.replaced = fun.head.id;
    op.replaces = value;
    return fun.body->substute(op);
  }
  else
    // throw error.
}

// Get the result of a substution on an element.
Element * Element::substute (SubstutionOp & subOp) const
{
  switch (type)
  {
  // Functions halt the substution if there head matches replaced, as it is
  // now a new variable bound to a different value. Otherwise continue.
  case function:
    if (fun.head.id == subOp.replaced)
      return new Element(*this);
    else
    {
      return ??;
    }
  // Applications pass the substution down to their two sides.
  case application:
  // A Variable that matched replaced returns replaces, otherwise a variable
  // returns a copy of itself.
  case variable:
    if (var.id == subOp.replaced)
      return new Element();
    else
      return new
  }
}

// Output Functions ==========================================================
// Write the element to the given stream.
std::ostream & Element::write (std::ostream & out) const
{
  switch (type)
  {
  case function:
    return fun.body->write(out << fun.head.id << '.');
  case application:
    app.rhs->write(out << '(')
    app.lhs->write(out << ' ')
    return out << ')';
  case variable:
    return out << var.id;
  }
}

