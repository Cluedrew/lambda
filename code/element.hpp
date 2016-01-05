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

class LambdaElement
{
  LambdaElement (ParseNode const *);
  /* Create a Element and any sub-Elements from a parse tree.
   * Params: A pointer to the root node of the parse tree.
   */

  LambdaElement (LambdaElement const &);
  /* Create a deep copy of an existing element.
   */

  ~LambdaElement () =0;

  bool isClosed () const;
  // Check to see if the element is closed.

  bool isClosedWith (std::vector<Variable const *> bounded) const;
  // Check to see if the element is closed within a given context.

  bool isExpression () const;
  // Check to see if the element is an expression.

  // Operation Functions
  virtual LambdaElement * evaluate () const;
  /* Get the result of an evaluation on the exprestion.
   * Effect:
   * Return:
   * Except:
   */

  virtual LambdaElement * apply (Element const & value) const;
  /* Get the result of an application on a function.
   * Params: A constaint reference to the value taken as the argument.
   * Effect:
   * Return:
   * Except:
   */

  virtual LambdaElement * substute (SubstutionOp const & subOp) const;
  /* Get the result of a substution on an element.
   * Params: A constaint reference to the substution to preform.
   * Effect:
   * Return:
   * Except:
   */

  virtual std::ostream & write (std::ostream &) const;
  /* Write the element to stream.
   * Params: A reference to an ostream to write to.
   * Effect: Writes to stream.
   * Return: A reference to the provided ostream.
   */
};

Element * parseNodeToElement (ParseNode const *);
/* Build an Element from a ParseNode.
 * Params: A pointer to the ParseNode.
 * Effect: Allocates a new Element (and any sub-Elements).
 * Return: A pointer to the new Element, caller must free.
 * Except: Throws std::invalid_argument if the conversion can't be made.
 */

class VariableElement : public Element
{
private:
  TextT id;
public:
  VariableElement (TextT);
  VariableElement (ParseNode const *);

  bool isClosed () const;
  /* Check to see if the element is closed.
   * Return: False, variables are never closed without context.
   */

  bool isClosedWith (std::vector<VariableElement const *> bounded) const;
  /* Check to see if the element is closed within a given context.
   * Params: A vector of pointers to bound variables.
   * Return: True if this variable matches one in the bounded vector.
   */

  LambdaElement * substute (SubstutionOp const & subOp) const;
  /* Get the result of a substution on an element.
   * Params: A constaint reference to the substution to preform.
   * Effect: Allocates a new Element.
   * Return: A pointer to the resulting Element, caller must free.
   */
};

class FunctionElement : public LambdaElement
{
  VariableElement head;
  LambdaElement * body;

public:
  FunctionElement (ParseNode const *);

  FunctionElement (TextT, LambdaElement *);

  LambdaElement * apply (LambdaElement const *);

  LambdaElement * substute (SubstutionOp const &);
};

class ApplicationElement : public LambdaElement
{
  LambdaElement * lhs;
  LambdaElement * rhs;

public:
  ApplicationElement (ParseNode const *);

  // ...
};

#endif//ELEMENT_HPP
