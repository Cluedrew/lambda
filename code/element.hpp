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
#include "parse-fwd.hpp"
class ParseNode;
class SubstutionOp;

struct Element
{
  // Internal Definitions:
  enum ElementType
  {
    function,
    variable,
    application
  };

  struct Variable
  {
    TextT id;
  };

  struct Function
  {
    Variable head;
    Element * body;
  };

  struct Application
  {
    Element * lhs;
    Element * rhs;
  };

  // Fields:
  ElementType type;

  union
  {
    Function fun;
    Variable var;
    Application app;
  };

  Element (ParseNode const *);
  /* Create a Element and any sub-Elements from a parse tree.
   * Params: A pointer to the root node of the parse tree.
   */

  Element (TextT);
  /* Create a variable element.
   */

  Element (TextT, Element *);
  /* Create a function element.
   */

  Element (Element *, Element *);
  /* Create an application element.
   */

  Element (Element const &);
  /* Create a deep copy of an existing element.
   */

  ~Element ();

  // Predicate Functions
  bool isFunction () const;
  // Check to see if the element is a function.

  bool isApplication () const;
  // Check to see if the element is an application.

  bool isVariable () const;
  // Check to see if the element is a variable.

  bool isClosed () const;
  // Check to see if the element is closed.

  bool isClosedWith (std::vector<Variable *> bounded) const;
  // Check to see if the element is closed within a given context.

  bool isExpression () const;
  // Check to see if the element is an expression.

  // Operation Functions
  Element * evaluate () const;
  /* Get the result of an evaluation on the exprestion.
   * Effect:
   * Return:
   * Except:
   */

  Element * apply (Element const & value) const
  /* Get the result of an application on a function.
   * Params: A constaint reference to the value taken as the argument.
   * Effect:
   * Return:
   * Except:
   */

  Element * substute (SubstutionOp const & subOp) const;
  /* Get the result of a substution on an element.
   * Params: A constaint reference to the substution to preform.
   * Effect:
   * Return:
   * Except:
   */

  std::ostream & write (std::ostream &) const;
  /* Write the element to stream.
   * Params: A reference to an ostream to write to.
   * Effect: Writes to stream.
   * Return: A reference to the provided ostream.
   */
};

#endif//ELEMENT_HPP
