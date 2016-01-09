#include "element.hpp"

// Implemention of the *Element classes; LambdaElement and its subclasses:
//   VariableElement, FunctionElement & ApplicationElement

#include <ostream>
#include <stdexcept>
#include "parse-node.hpp"
#include "substution-op.hpp"
#include "token.hpp"
#include "symbol.hpp"



// Thoughts For Upgrades : Implementation Below:

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

/* Might it be worth cashing isClosed and isExpression Booleans?
 * is%(), is%Flag, is%Dirty, update%(), outofDate%()
 *
 * bool is%()
 * {
 *   if (is%Dirty)
 *   {
 *     is%Flag = update%();
 *     is%Dirty = false;
 *   }
 *   return is%Flag;
 * }
 *
 * // Or declaring the flags as mutable and the above const.
 * bool is%() const
 * {
 *   if (is%Dirty)
 *     return update%();
 *   else
 *     return is%Flag;
 * }
 *
 * void outOfDate%()
 * {
 *   is%Dirty = true;
 * }
 *
 * virtual update%() const =0;
 *
 * These (and the variables) will be in LambdaElement. Subclasses just have to
 * implement update%(). clone will have to be updated so this data is copied
 * over. If the program runs to slowly I'll do it.
 */




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
  if (nullptr == node)
    throw std::invalid_argument("fromParseRoot: invalid NULL pointer.");
  if (SymbolT::INPUT != node->getHead())
    throw std::invalid_argument("fromParseRoot: Not an INPUT node.");

  return fromParseRoot(node->child(0));
}

// Create a new element from a parse tree.
LambdaElement * LambdaElement::fromParseTree (ParseNode const * node)
{
  if (nullptr == node)
    throw std::invalid_argument("fromParseTree: invalid NULL pointer.");
  if (SymbolT::ELEMENT != node->getHead())
    throw std::invalid_argument("fromParseTree: Not an ELEMENT node.");

  if (1 == node->size())
  {
    switch (node->getISymbol(0))
    {
    case SymbolT::variable:
      return new VariableElement(node->child(0));
    case SymbolT::FUNCTION:
      return new FunctionElement(node->child(0));
    case SymbolT::APPLICATION:
      return new ApplicationElement(node->child(0));
    default:
      break;
    }
  }

  throw std::invalid_argument("fromParseTree: invalid ELEMENT node.");
}



// VariableElement ===========================================================
// Create a new element from a parse tree.
VariableElement::VariableElement (ParseNode const * node) :
  LambdaElement(), id()
{
  if (nullptr == node)
    throw std::invalid_argument("VariableElement: invalid NULL pointer.");
  if (SymbolT::variable != node->getHead())
    throw std::invalid_argument("VariableElement: Not a variable node.");

  id = node->getText();
}

// Manual constructor, create the element from its parts.
VariableElement::VariableElement (TextT id) :
  LambdaElement(), id(id)
{}

// Clone, create a deep copy.
LambdaElement * VariableElement::clone () const
{
  return new VariableElement(id);
}

VariableElement::~VariableElement ()
{}



// Check to see if the element is closed.
bool VariableElement::isClosed () const
{
  return false;
}

// Check to see if the element is closed within a given context.
bool VariableElement::isClosedWith
    (std::vector<VariableElement const *> bounded) const
{
  for (std::vector<VariableElement const *>::const_iterator it
       = bounded.cbegin() ; it != bounded.cend() ; ++it)
  {
    if (id == (**it).id)
      return true;
  }
  return false;
}

// Check to see if the element is an expression.
bool VariableElement::isExpression () const
{
  return false;
}



// Get the result of an evaluation on the exprestion.
LambdaElement * VariableElement::evaluate () const
{
  throw std::logic_error("evaluate: VariableElement is not an expression.");
}

// Get the result of a substution on an element.
LambdaElement * VariableElement::substute (SubstutionOp const & subOp) const
{
  if (subOp.replaced == id)
    return subOp.replaces.clone();
  else
    return this->clone();
}

// Write the element to stream.
std::ostream & VariableElement::write (std::ostream & out) const
{
  return out << id;
}



// Convert the element to text.
VariableElement::operator TextT () const
{
  return id;
}

// Check for equality between two VariableElements.
bool VariableElement::operator== (VariableElement const & other) const
{
  return id == other.id;
}



// FunctionElement ===========================================================
// Create a new element from a parse tree.
FunctionElement::FunctionElement (ParseNode const * node) :
  LambdaElement(), head('\0'), body(nullptr)
{
  if (nullptr == node)
    throw std::invalid_argument("FunctionElement: invalid NULL pointer.");
  if (SymbolT::FUNCTION != node->getHead())
    throw std::invalid_argument("FunctionElement: Not a FUNCTION node.");

  head = node->child(0)->getText();
  body = LambdaElement::fromParseTree(node->child(2));
}

// Manual constructor, create the element from its parts.
FunctionElement::FunctionElement (TextT head, LambdaElement * body) :
  LambdaElement(), head(head), body(body)
{
  if (nullptr == body)
    throw std::invalid_argument("FunctionElement: invalid NULL pointer.");
}

// Clone, create a deep copy.
LambdaElement * FunctionElement::clone () const
{
  LambdaElement * tmpBody = body->clone();

  try
  {
    return new FunctionElement(TextT(head), tmpBody);
  }
  catch (...)
  {
    delete tmpBody;
    throw;
  }
}

// Deconstrutor
FunctionElement::~FunctionElement ()
{
  delete body;
}



// Check to see if the element is closed.
bool FunctionElement::isClosed () const
{
  return body->isClosedWith(Context(1, &head));
}

// Check to see if the element is closed within a given context.
bool FunctionElement::isClosedWith
    (std::vector<VariableElement const *> bounded) const
{
  for (unsigned int i = 0 ; i < bounded.size() ; ++i)
  {
    if (*bounded[i] == head)
    {
      bounded[i] = &head;
      return body->isClosedWith(bounded);
    }
  }
  bounded.push_back(&head);
  return body->isClosedWith(bounded);
}

// Check to see if the element is an expression.
bool FunctionElement::isExpression () const
{
  return isClosed();
}



// Get the result of an evaluation on the exprestion.
LambdaElement * FunctionElement::evaluate () const
{
  if (isExpression())
    return clone();
  else
    throw std::logic_error("evaluate: FunctionElement is not an expression.");

}

// Get the result of an application on a function.
LambdaElement * FunctionElement::apply (LambdaElement const * arg) const
{
  return body->substute(SubstutionOp(TextT(head), *arg));
}

// Get the result of a substution on an element.
LambdaElement * FunctionElement::substute (SubstutionOp const & subOp) const
{
  if (TextT(head) == subOp.replaced)
    return clone();
  else
  {
    LambdaElement * tmpBody = body->substute(subOp);
    try
    {
      return new FunctionElement(TextT(head), tmpBody);
    }
    catch (...)
    {
      delete tmpBody;
      throw;
    }
  }
}

// Write the element to stream.
std::ostream & FunctionElement::write (std::ostream & out) const
{
  head.write(out);
  out << '.';
  body->write(out);
  return out;
}



// ApplicationElement ========================================================
// Create a new element from a parse tree.
ApplicationElement::ApplicationElement (ParseNode const * node) :
  LambdaElement(), lhs(nullptr), rhs(nullptr)
{
  if (nullptr == node)
    throw std::invalid_argument("ApplicationElement: invalid NULL pointer.");
  if (SymbolT::APPLICATION != node->getHead())
    throw std::invalid_argument("ApplicationElement: Not an APPLICATION node.");

  LambdaElement * tmpLhs = LambdaElement::fromParseTree(node->child(1));
  try
  {
    rhs = LambdaElement::fromParseTree(node->child(2));
  }
  catch (...)
  {
    delete tmpLhs;
    throw;
  }
  lhs = tmpLhs;
}

// Manual constructor, create the element from its parts.
ApplicationElement::ApplicationElement
    (LambdaElement * lhs, LambdaElement * rhs) :
  LambdaElement(), lhs(lhs), rhs(rhs)
{
  if (nullptr == lhs || nullptr == rhs)
    throw std::invalid_argument("ApplicationElement: invalid NULL pointer.");
}

// Clone, create a deep copy.
LambdaElement * ApplicationElement::clone () const
{
  LambdaElement * tmpLhs = lhs->clone();
  try
  {
    LambdaElement * tmpRhs = rhs->clone();
    try
    {
      return new ApplicationElement(tmpLhs, tmpRhs);
    }
    catch (...)
    {
      delete tmpRhs;
      throw;
    }
  }
  catch (...)
  {
    delete tmpLhs;
    throw;
  }
}

// Deconstrutor
ApplicationElement::~ApplicationElement ()
{
  delete lhs;
  delete rhs;
}



// Check to see if the element is closed.
bool ApplicationElement::isClosed () const
{
  return lhs->isClosed() && rhs->isClosed();
}

// Check to see if the element is closed within a given context.
bool ApplicationElement::isClosedWith
    (std::vector<VariableElement const *> bounded) const
{
  return lhs->isClosedWith(bounded) && rhs->isClosedWith(bounded);
}

// Check to see if the element is an expression.
bool ApplicationElement::isExpression () const
{
  return lhs->isExpression() && rhs->isExpression();
}



// Get the result of an evaluation on the exprestion.
LambdaElement * ApplicationElement::evaluate () const
{
  if (!isExpression())
    throw std::logic_error(
        "evaluate: ApplicationElement is not an expression.");

  LambdaElement * tmpLhs = lhs->evaluate();
  try
  {
    LambdaElement * tmpRhs = rhs->evaluate();
    try
    {
      FunctionElement * func = dynamic_cast<FunctionElement*>(tmpLhs);
      if (nullptr == func)
        return new ApplicationElement(tmpLhs, tmpRhs);
      else
      {
        LambdaElement * fin = func->apply(tmpRhs);
        delete tmpLhs;
        delete tmpRhs;
        return fin;
      }
    }
    catch (...)
    {
      delete tmpRhs;
      throw;
    }
  }
  catch (...)
  {
    delete tmpLhs;
    throw;
  }
}

// Get the result of a substution on an element.
LambdaElement * ApplicationElement::substute (SubstutionOp const & subOp)const
{
  LambdaElement * tmpLhs = lhs->substute(subOp);
  try
  {
    LambdaElement * tmpRhs = rhs->substute(subOp);
    try
    {
      return new ApplicationElement(tmpLhs, tmpRhs);
    }
    catch (...)
    {
      delete tmpRhs;
      throw;
    }
  }
  catch (...)
  {
    delete tmpLhs;
    throw;
  }
}

// Write the element to stream.
std::ostream & ApplicationElement::write (std::ostream & out) const
{
  out << '(';
  lhs->write(out);
  out << ' ';
  rhs->write(out);
  out << ')';
  return out;
}
