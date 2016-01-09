#ifndef ELEMENT_HPP
#define ELEMENT_HPP

/* The Element class is a union of structures, each one repersenting the three
 * types of elements in lambda calculus. Hence it repersents any element of
 * lambda calculus.
 *
 * All three sub-types have the same interface, but some member functions are
 * only valid on some sub-types. So it is sort of like a compacted class-type
 * tree.
 */

#include <ostream>
#include <vector>
#include "parse-fwd.hpp"
class SubstutionOp;

class VariableElement;
typedef std::vector<VariableElement const *> Context;

class LambdaElement
{
private:
protected:
  LambdaElement ();
  /* Default Constructor
   */

public:
  virtual LambdaElement * clone () const =0;
  /* Clone, create a deep copy.
   * Effect: Allocates a new element and its sub-elements.
   * Return: Pointer to new LambdaElement, caller must free.
   */

  virtual ~LambdaElement () =0;

  virtual bool isClosed () const =0;
  /* Check to see if the element is closed.
   * Return: True if all variables in the element are bound within the
   *   element.
   */

  virtual bool isClosedWith (Context bounded) const =0;
  /* Check to see if the element is closed within a given context.
   * Params: TODO A vector of pointers to bound variables.
   * Return: True if all variables are in the element are bound to variables
   *   in the vector or within the element.
   */

  virtual bool isExpression () const =0;
  /* Check to see if the element is an expression.
   * Return: True if the element can be evaluated, false otherwise.
   */

  virtual LambdaElement * evaluate () const =0;
  /* Get the result of an evaluation on the expression.
   * Effect: Allocates a new element and its sub-elements.
   * Return: Pointer to the new LambdaElement, caller must free.
   * Except: Throws std::logic_error if not an expression
   */

  virtual LambdaElement * substute (SubstutionOp const & subOp) const =0;
  /* Get the result of a substution on an element.
   * Params: A constaint reference to the substution to preform.
   * Effect: Allocates a new element and its sub-elements.
   * Return: Pointer to the new LambdaElement, caller must frsee.
   * Except:
   */

  virtual std::ostream & write (std::ostream &) const =0;
  /* Write the element to stream.
   * Params: A reference to an ostream to write to.
   * Effect: Writes to stream.
   * Return: A reference to the provided ostream.
   */

  static LambdaElement * fromParseRoot (ParseNode const *);
  /* Create a new element from the root of a parse tree.
   * Params: Pointer to a 'INPUT' ParseNode.
   */

  static LambdaElement * fromParseTree (ParseNode const *);
  /* Create a new element from a parse tree.
   * Params: Pointer to a 'ELEMENT' ParseNode.
   */
};



class VariableElement : public LambdaElement
{
private:
  TextT id;

public:
  VariableElement (ParseNode const *);
  /* Create a new element from a parse tree.
   * Params: Pointer to a 'variable' ParseNode.
   */

  VariableElement (TextT);
  /* Manual constructor, create the element from its parts.
   * Params: The TextT identifier for the variable.
   */

  LambdaElement * clone () const;
  /* Clone, create a deep copy.
   * Effect: Allocates a new element and its sub-elements.
   * Return: Pointer to new LambdaElement, caller must free.
   */

  virtual ~VariableElement ();

  bool isClosed () const;
  /* Check to see if the element is closed.
   * Return: False, variables are never closed without context.
   */

  bool isClosedWith (Context bounded) const;
  /* Check to see if the element is closed within a given context.
   * Params: A vector of pointers to bound variables.
   * Return: True if this variable matches one in the bounded vector.
   */

  bool isExpression () const;
  /* Check to see if the element is an expression.
   * Return: False
   */

  LambdaElement * evaluate () const;
  /* Get the result of an evaluation on the expression.
   * Effect: Allocates a new element and its sub-elements.
   * Return: Never returns
   * Except: Throws std::logic_error if not an expression
   */

  LambdaElement * substute (SubstutionOp const & subOp) const;
  /* Get the result of a substution on an element.
   * Params: A constaint reference to the substution to preform.
   * Effect: Allocates a new element and its sub-elements.
   * Return: A pointer to the resulting LambdaElement, caller must free.
   */

  std::ostream & write (std::ostream &) const;
  /* Write the element to stream.
   * Params: A reference to an ostream to write to.
   * Effect: Writes to stream.
   * Return: A reference to the provided ostream.
   */

  operator TextT () const;
  /* Convert the element to text.
   * Return: The variable id.
   */

  bool operator== (VariableElement const &) const;
  /* Check for equality between two VariableElements.
   * Params: A constant reference to another VariableElement.
   * Return: True if the two objects have the same id.
   */
};

class FunctionElement : public LambdaElement
{
private:
  VariableElement head;
  LambdaElement * body;

public:
  FunctionElement (ParseNode const *);
  /* Create a new element from a parse tree.
   * Params: Pointer to a 'FUNCTION' ParseNode.
   * Effect: Allocates sub-elements.
   */

  FunctionElement (TextT, LambdaElement *);
  /* Manual constructor, create the element from its parts.
   * Params: The TextT identifier for the parameter and a pointer
   *   to the body. The element takes ownership of the body.
   */

  LambdaElement * clone () const;
  /* Clone, create a deep copy.
   * Effect: Allocates a new element and its sub-elements.
   * Return: Pointer to new LambdaElement, caller must free.
   */

  virtual ~FunctionElement ();

  bool isClosed () const;
  /* Check to see if the element is closed.
   * Return: True if all variables in the function body are bound.
   */

  bool isClosedWith (Context bounded) const;
  /* Check to see if the element is closed within a given context.
   * Params: A vector of pointers to bound variables.
   * Return: True if all variables in the function body matches one in the
   *   bounded vector or the function's head.
   */

  bool isExpression () const;
  /* Check to see if the element is an expression.
   * Return: True if function is closed.
   */

  LambdaElement * evaluate () const;
  /* Get the result of an evaluation on the expression.
   * Effect: Allocates a new element and its sub-elements.
   * Return: Pointer to the new LambdaElement, caller must free.
   * Except: Throws std::logic_error if not an expression
   */

  LambdaElement * apply (LambdaElement const *) const;
  /* Get the result of an application on a function.
   * Params: A constaint reference to the value taken as the argument.
   * Effect: Allocates a new element and its sub-elements.
   * Return: Pointer to the new LambdaElement, caller must frsee.
   * Except:
   */

  LambdaElement * substute (SubstutionOp const &) const;
  /* Get the result of a substution on an element.
   * Params: A constaint reference to the substution to preform.
   * Effect: Allocates a new element and its sub-elements.
   * Return: A pointer to the resulting LambdaElement, caller must free.
   */

  std::ostream & write (std::ostream &) const;
  /* Write the element to stream.
   * Params: A reference to an ostream to write to.
   * Effect: Writes to stream.
   * Return: A reference to the provided ostream.
   */
};

class ApplicationElement : public LambdaElement
{
  LambdaElement * lhs;
  LambdaElement * rhs;

public:
  ApplicationElement (ParseNode const *);
  /* Create a new element from a parse tree.
   * Params: Pointer to an 'APPLICATION' ParseNode.
   * Effect: Allocates sub-elements.
   */

  ApplicationElement (LambdaElement * lhs, LambdaElement * rhs);
  /* Manual constructor, create the element from its parts.
   * Params: Pointers to the left and right hand side of the application.
   *   The element takes ownership of the pointers.
   */

  LambdaElement * clone () const;
  /* Clone, create a deep copy.
   * Effect: Allocates a new element and its sub-elements.
   * Return: Pointer to new LambdaElement, caller must free.
   */

  virtual ~ApplicationElement ();

  bool isClosed () const;
  /* Check to see if the element is closed.
   * Return: True if the left and right hand sides are closed.
   */

  bool isClosedWith (Context bounded) const;
  /* Check to see if the element is closed within a given context.
   * Params: A vector of pointers to bound variables.
   * Return: True if this variable matches one in the bounded vector.
   */

  bool isExpression () const;
  /* Check to see if the element is an expression.
   * Return: True if the left and right hand sides are expressions.
   */

  LambdaElement * evaluate () const;
  /* Get the result of an evaluation on the expression.
   * Effect: Allocates a new element and its sub-elements.
   * Return: Pointer to the new LambdaElement, caller must free.
   * Except: Throws std::logic_error if not an expression
   */

  LambdaElement * substute (SubstutionOp const &) const;
  /* Get the result of a substution on an element.
   * Params: A constaint reference to the substution to preform.
   * Effect: Allocates a new element and its sub-elements.
   * Return: A pointer to the resulting LambdaElement, caller must free.
   */

  std::ostream & write (std::ostream &) const;
  /* Write the element to stream.
   * Params: A reference to an ostream to write to.
   * Effect: Writes to stream.
   * Return: A reference to the provided ostream.
   */
};

#endif//ELEMENT_HPP
