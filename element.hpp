#ifndef ELEMENT_HPP
#define ELEMENT_HPP

/* Reperesents an element in my lambda caluclator.
 */

#include <string>
#include <iosfwd>
struct SubstutionOp;

class Element
{
private:
  char core;
  Element * lhs;
  Element * rhs;

  void construct (char const *, int &);

protected:
public:
  Element (char, Element *, Element *);
  // By field constructor, for internal use only.

  Element (char);
  /* Parameter constructor, short cut to the given parameter.
   * Params: A char value to repersent the parameter.
   */

public:
  Element (char const *);
  /* Create an element from a string.
   * Params: A pointer to an array of characters.
   * NOTE: Currantly the syntax is very unforgiving.
   */

  Element (char const *, int &);
  /* Create an element from part of a string.
   * Params: A pointer to an array of characters an a reference to a offset,
   *   reading will start from the offset.
   * Effect: Advances the offset to one past the end of the characters used
   *   to create this element.
   * NOTE: Currantly the syntax is very unforgiving.
   */

  // Copy Constructor (Deep Copy)
  Element (Element const &);

  virtual ~Element ();

  bool isFunction () const;
  bool isEvaluation () const;
  bool isParameter () const;
  /* Check to see if the element is of the given type.
   * Return: True if the element is of the type in the function name,
   *   false otherwise.
   */

  bool isClosed () const;
  /* Check to see if the element is closed.
   * Return: True if closed, false otherwise. For each type of element:
   * - Parameters are not closed and always returns false.
   * - Evaluations are closed if both of their sides are closed.
   * - Functions are closed if all parameters within them are bound to a
   *   function header above them.
   */
  bool isClosedWith (std::string) const;
  /* Check to see if the element is closed within a given context.
   * Params: A list of bound parameters within the given context.
   * Return: True if closed, false otherwise. For each type of element:
   * - Parameters are closed if they appear in the list.
   * - Evaluations are closed if both of their sides are closed.
   * - Functions are closed if all parameters within them are bound to a
   *   parameter within the list or a header within the function.
   */

  bool isExpretion () const;
  /* Check to see if the element is an expretion.
   * Return: By the type of element:
   * - Parameters are not exprestions and return false.
   * - Evaluations are exprestions if both of their sides are expretions.
   * - Functions are exprestions if they are closed, equivilant to isClosed().
   */

  Element * evaluate () const;
  /* Get the result of an evaluation on the exprestion.
   * Return: A pointer to the element that results from the evaluation. Caller
   *   must free.
   */
  Element * apply (Element *) const;
  /* Get the result of an application on a function.
   * Params:
   * Return: A pointer to the element that results from the application.
   *   Caller must free.
   */
  Element * substute (SubstutionOp &) const;
  /* Get the result of a substution on an element.
   * Params:
   * Return: A pointer to the element that results from the substution. Caller
   *   must free.
   */

  void print (std::ostream &);
  /* Print the element to the given stream.
   * Params: A standard output stream to print to.
   * Effect: Characters are sent to the stream.
   */
};

#endif//ELEMENT_HPP
