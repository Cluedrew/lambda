#ifndef RULE_ITEM_HPP
#define RULE_ITEM_HPP

// The production Rule structure and the Item there of.
// Not that the constrants on Item are not enforced.

#include <iosfwd>
#include <vector>
#include "parse-fwd.hpp"
struct Item;

// The production Rule
struct Rule
{
  SymbolT lhs;
  std::vector<SymbolT> rhs;

  Rule(SymbolT lhs, std::vector<SymbolT> rhs);
  /* Create a new Rule with all fields specified.
   * Params: Field values in order.
   */

  Rule();
  // Create a new uninitalized Rule.

  unsigned int cr () const;
  /* Count Right: Get the size of the right hand side.
   * Return: rhs.size()
   */

  bool operator< (Rule const &) const;
  bool operator== (Rule const &) const;
  bool operator!= (Rule const &) const;

  Item getFresh () const;
  /* Return a fresh item from this production Rule.
   * Return: An Item with lhs & rhs equal to the Rule's fields and place = 0.
   */
};

// The Item
struct Item
{
  SymbolT lhs;
  std::vector<SymbolT> rhs;
  // place should be in the range of [0..cr()]
  unsigned int place;

  Item(SymbolT lhs, std::vector<SymbolT> rhs, unsigned int place);
  /* Create a new Item with all fields specified.
   * Params: Field values in order.
   */

  Item();
  // Create a new uninitalized Item.

  unsigned int cr () const;
  /* Count Right: Get the size of the right hand side.
   * Return: rhs.size()
   */

  bool operator< (Item const &) const;
  bool operator== (Item const &) const;
  bool operator!= (Item const &) const;

  Rule getBase () const;
  /* Return the base production Rule of the item.
   * Return: A new Rule with lhs & rhs equal to the Item's.
   */

  Item getNext () const;
  /* Return a new Item that is this item progressed by one.
   * Return: A new Item equal to this one except that the place field
   *   is incremented by one.
   */
};

std::ostream & operator<< (std::ostream &, Rule const &);
std::ostream & operator<< (std::ostream &, Item const &);
/* Print the Rule or Item to the given stream.
 * Params: A reference to the stream to be printed to and a constant
 *   refrence to the Rule or Item to be printed.
 * Effect: Characters sent to stream buffer.
 * Return: A reference to the stream printed to.
 * Format: See Rule/Item Format
 */

std::istream & operator>> (std::istream &, Rule &);
std::istream & operator>> (std::istream &, Item &);
/* Read a Rule or Item from a stream.
 * Params: A reference to the stream to read from and a reference to
 *   the variable to store the Rule or Item in.
 * Effect: Stream is read from, value in the container will be updated and
 *   on error stream's badbit is set.
 * Return: A reference to the stream provided.
 * Format: See Rule/Item Format
 */

/* Rule/Item Format:
 *   LHS ( RHS[0] ... )
 * Where LHS and RHS[i] are all SymbolT. Also note that Items have a star (*)
 * before RHS[place], if place = N than the star is between the last symbol
 * and the ')'. All symbols, '(', ')' & '*' should all be seperated by white
 * space.
 */

#endif//RULE_ITEM_HPP
