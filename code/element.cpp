#include "element.hpp"

// Implemention of the *Element classes; LambdaElement and its subclasses:
//   VariableElement, FunctionElement & ApplicationElement

#include <ostream>
#include <stdexcept>
#include "parse-node.hpp"
#include "substution-op.hpp"
#include "token.hpp"
#include "symbol.hpp"



// LambdaElement =============================================================
// Default Constructor
LambdaElement::LambdaElement ()
{}

// Deconstructor
LambdaElement::~LambdaElement ()
{}



// Create a new element from the root of a parse tree.
LambdaElement * LambdaElement::fromParseRoot (ParseNode const * node)
{
  // Params: Pointer to a 'INPUT' ParseNode.
  ! // TODO
}

// Create a new element from a parse tree.
LambdaElement * LambdaElement::fromParseTree (ParseNode const * node)
{
  // Params: Pointer to a 'ELEMENT' ParseNode.
  ! // TODO
}



// VariableElement ===========================================================
// Create a new element from a parse tree.
VariableElement::VariableElement (ParseNode const * node) :
  LambdaElement(), id()
{
  ! // TODO
}

// Manual constructor, create the element from its parts.
VariableElement::VariableElement (TextT id) :
  LambdaElement(), id(id)
{}

// Clone, create a deep copy.
LambdaElement * clone ()
{
  return new VariableElement(id);
}

VariableElement::~VariableElement () {}



// Check to see if the element is closed.
bool VariableElement::isClosed () const
{
  return false;
}

// Check to see if the element is closed within a given context.
bool VariableElement::isClosedWith
    (std::vector<VariableElement const *> bounded) const
{
  ! // TODO
}

// Check to see if the element is an expression.
bool VariableElement::isExpression () const
{
  ! // TODO
}



// Get the result of a substution on an element.
LambdaElement * VariableElement::substute (SubstutionOp const & subOp) const
{
  ! // TODO
}

// Write the element to stream.
std::ostream & VariableElement::write (std::ostream &) const
{
  ! // TODO
}



// FunctionElement ===========================================================
// Create a new element from a parse tree.
FunctionElement::FunctionElement (ParseNode const * node)
  LambdaElement(), head(), body(nullptr)
{
  ! // TODO
}

// Manual constructor, create the element from its parts.
FunctionElement::FunctionElement (TextT head, LambdaElement * body)
  LambdaElement(), head(head), body(body)
{}

// Clone, create a deep copy.
LambdaElement * FunctionElement::clone ()
{
  ! // TODO
}

// Deconstrutor
FunctionElement::~FunctionElement ()
{
  ! // TODO
}

// Check to see if the element is closed.
bool FunctionElement::isClosed () const
{
  ! // TODO
}

// Check to see if the element is closed within a given context.
bool FunctionElement::isClosedWith
    (std::vector<VariableElement const *> bounded) const
{
  ! // TODO
}

// Check to see if the element is an expression.
bool FunctionElement::isExpression () const
{
  ! // TODO
}

// Get the result of an application on a function.
LambdaElement * FunctionElement::apply (LambdaElement const * arg)
{
  ! // TODO
}

// Get the result of a substution on an element.
LambdaElement * FunctionElement::substute (SubstutionOp const & subOp)
{
  ! // TODO
}

// Write the element to stream.
std::ostream & FunctionElement::write (std::ostream & out) const
{
  ! // TODO
}



// ApplicationElement ========================================================
// Create a new element from a parse tree.
ApplicationElement::ApplicationElement (ParseNode const * node)
  ApplicationElement(), lhs(), rhs()
{
  ! // TODO
}

// Manual constructor, create the element from its parts.
ApplicationElement::ApplicationElement
    (LambdaElement * lhs, LambdaElement * rhs)
  ApplicationElement(), lhs(lhs), rhs(rhs)
{}

// Clone, create a deep copy.
LambdaElement * ApplicationElement::clone ()
{
  ! // TODO
}

// Deconstrutor
ApplicationElement::~ApplicationElement ()
{
  ! // TODO
}

// Check to see if the element is closed.
bool ApplicationElement::isClosed () const
{
  ! // TODO
}

// Check to see if the element is closed within a given context.
bool ApplicationElement::isClosedWith
    (std::vector<VariableElement const *> bounded) const
{
  ! // TODO
}

// Check to see if the element is an expression.
bool ApplicationElement::isExpression () const
{
  ! // TODO
}

// Get the result of a substution on an element.
LambdaElement * ApplicationElement::substute (SubstutionOp const & subOp)
{
  ! // TODO
}

// Write the element to stream.
std::ostream & ApplicationElement::write (std::ostream & out) const
{
  ! // TODO
}



// ===========================================================================
// Old Implementation ========================================================
// Constructors and Deconstructor ============================================
// Construct an element from a ParseNode.
Element::Element (ParseNode const * node)
{
  if (node->getHead() != SymbolEnum::ELEMENT)
  {
    // error: Might try and add more information.
    throw std::invalid_argument("Element constructor recived bad node");
  }

  ParseNode const * inner = node->child(0);
  // Currently, to checking the first symbol is enough know exactly what
  //   Rule is being used.
  switch (inner->getHead())
  {
  case SymbolEnum::FUNCTION:
    // ELEMENT FUNCTION / FUNCTION variable dot ELEMENT
    type = function;
    fun.head.id = inner->child(0)->getText();
    fun.body = new Element(inner->child(2));
    break;
  case SymbolEnum::APPLICATION:
    // ELEMENT APPLICATION / APPLICATION openApp ELEMENT ELEMENT closeApp
    type = application;
    app.lhs = new Element(inner->child(1));
    app.rhs = new Element(inner->child(2));
    break;
  case SymbolEnum::variable:
    // ELEMENT variable
    type = variable;
    var.id = inner->getText();
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
    fun.body = new Element(*other.fun.body);
    break;
  case application:
    app.lhs = new Element(*other.app.lhs);
    app.rhs = new Element(*other.app.rhs);
    break;
  default:
    var.id = other.var.id;
    break;
  }
}

// Create a variable element.
Element::Element (TextT text) :
  type(variable), var(Variable{text})
{}

// Create a function element.
Element::Element (TextT head, Element * body) :
  type(function), fun(Function{head, body})
{}

// Create an application element.
Element::Element (Element * lhs, Element * rhs) :
  type(application), app(Application{lhs, rhs})
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
  case variable:
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
    return fun.body->isClosedWith(
        std::vector<Variable const *>(1, &fun.head));
  // Evaluations should check the right and left hand sides.
  case application:
    return app.lhs->isClosed() && app.rhs->isClosed();
  // Parameters are not closed without context.
  case variable:
  default:
    return false;
  }
}

// Check to see if the element is closed within a given context.
bool Element::isClosedWith (std::vector<Variable const *> bounded) const
{
  switch (type)
  {
  // Functions change the context before passing it down.
  case function:
    for (unsigned int i = 0 ; i < bounded.size() ; ++i)
      if (bounded[i]->id == fun.head.id)
      {
        bounded[i] = &fun.head;
        return fun.body->isClosedWith(bounded);
      }
    bounded.push_back(&fun.head);
    return fun.body->isClosedWith(bounded);
  // Applications just pass down to the two sides.
  case application:
    return (app.lhs->isClosedWith(bounded) && app.rhs->isClosedWith(bounded));
  // Variables finally check to see if they are bounded.
  case variable:
  default:
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
    return isClosed();
  // Applications are expressions if their two sides are expressions.
  case application:
    return app.lhs->isExpression() && app.rhs->isExpression();
  // Variables are not expressions.
  case variable:
  default:
    return false;
  }
}

// Operation Functions =======================================================

// Get the result of an evaluation on the exprestion.
Element * Element::evaluate () const
{
  if (!isExpression())
    throw std::logic_error("Element: evaluate: object is not an expression.");

  // Closed Functions are there own result.
  if (isFunction())
    return new Element(*this);
  // Applications apply the rhs to the lhs.
  else
  {
    Element * lhsResult = app.lhs->evaluate();
    Element * rhsResult = app.rhs->evaluate();
    Element * finResult = lhsResult->apply(*rhsResult);
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
    SubstutionOp op(fun.head.id, value);
    return fun.body->substute(op);
  }
  else
    throw std::logic_error("Element: apply: non-function called.");
}

// Get the result of a substution on an element.
Element * Element::substute (SubstutionOp const & subOp) const
{
  switch (type)
  {
  // Functions halt the substution if there head matches replaced, as it is
  // now a new variable bound to a different value. Otherwise continue.
  case function:
    if (fun.head.id == subOp.replaced)
      return new Element(*this);
    else
      return new Element(fun.head.id, fun.body->substute(subOp));
  // Applications pass the substution down to their two sides.
  case application:
  {
    Element * lhsResult = app.lhs->substute(subOp);
    Element * rhsResult = app.rhs->substute(subOp);
    return new Element(lhsResult, rhsResult);
  }
  // A Variable that matched replaced returns replaces, otherwise a variable
  // returns a copy of itself.
  case variable:
  default:
    if (var.id == subOp.replaced)
      return new Element(subOp.replaced);
    else
      return new Element(*this);
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
    app.rhs->write(out << '(');
    app.lhs->write(out << ' ');
    return out << ')';
  case variable:
  default:
    return out << var.id;
  }
}



// Build an Element from a ParseNode.
Element * parseNodeToElement (ParseNode const * node)
{
  // First a basic check.
  if (SymbolEnum::ELEMENT != node->getHead())
    throw std::invalid_argument("parseNodeToElement: node not an ELEMENT.");
  // Otherwise make the approprate node.
  else
    return new Element(node);
/*
  ParseNode const * cNode = node->child(0);
   if (SymbolEnum::variable == cNode->getHead())
    return new VariableElement(cNode);
  else if (SymbolEnum::FUNCTION == cNode->getHead())
    return new FunctionElement(cNode);
  else if (SymbolEnum::APPLICATION == cNode->getHead())
    return new ApplicationElement(cNode);
  else
    throw std::invalid_argument("");
*/
}

#if 0
// Should I switch the implementation over?
// I am basically just mimicing pointers all the time so the inline space
// doesn't matter anyways. ...It was an interesting thought experement.
class VariableElement : public Element
{}
class FunctionElement : public Element
{}
class ApplicationElement : public Element
{}

VariableElement::VariableElement (ParseNode const * node) :
  id()
{
  if (SymbolEnum::variable != node->getHead())
    throw std::invalid_argument("VariableElement: node not a variable.");

  id = node->getText();
}

FunctionElement::FunctionElement (ParseNode const * node) :
  head(), body(nullptr)
{
  if (SymbolEnum::FUNCTION != node->getHead())
    throw std::invalid_argument("FunctionElement: node not a FUNCTION.");
}

ApplicationElement::ApplicationElement (ParseNode const * node) :
  lhs(nullptr), rhs(nullptr)
{
  if (SymbolEnum::APPLICATION != node->getHead())
    throw std::invalid_argument("VariableElement: node not a variable.");
}

#endif
