#ifndef ELEMENT_HPP
#define ELEMENT_HPP

/* Reperesents an element in my lambda caluclator.
 */

#include <string>

// ____
struct SubstutionOp
{
  // [ replaces / replaced ]
  char replaced;
  Element * replaces;
};

// ^^^^

class Element
{
private:
  char core;
  Element * lhs;
  Element * rhs;

  void construct (char const *, int &);

protected:
  Element (char);

public:
  Element (char const *);
  Element (char const *, int &);

  Element (Element const &);

  virtual ~Element ();

  bool isFunction ();
  bool isEvaluation ();
  bool isParameter ();

  bool isClosed ();
  bool isClosedWith (std::string);

  bool isExpretion ();

  Element * evaluate ();
  Element * apply (Element *);
  Element * substute (SubstutionOp &);
};

#endif//ELEMENT_HPP
