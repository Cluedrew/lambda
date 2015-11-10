#ifndef ELEMENT_HPP
#define ELEMENT_HPP

// Will replace "element.hpp" as the parser progresses.

/* A new approch to the Elements. Not only is it devided up a little bit
 * differently, but it is designed to take advantage of the parse tree.
 */

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
    char id;
  };

  struct Function
  {
    Variable head;
    Element * Body;
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

  Element (ParseNode const &);
  /* Create a Element and any sub-Elements from a parse tree.
   * Params: A constant reference to the root node of the parse tree.
   */

  // The deconstructor should switch on type.
  ~Element ();
};

#endif//ELEMENT_HPP
