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
  unsigned int cr () const; // count right, rhs.size()
  bool operator< (Rule const &) const;

  // Return a fresh item from this production Rule.
  Item getFresh ();
};

// The Item
struct Item // : public Rule
{
  SymbolT lhs;
  std::vector<SymbolT> rhs;
  unsigned int place; // In the range of [0..cr()]
  unsigned int cr () const; // count right, rhs.size()
  bool operator< (Item const &) const;

  // Return the base production Rule of the item.
  Rule getBase ();
  // Return a new Item that is this item progressed by one.
  Item getNext ();
};

std::ostream & operator<< (std::ostream &, Rule const &);
std::ostream & operator<< (std::ostream &, Item const &);
/* Print the Rule or Item to the given stream.
 * Params: A reference to the stream to be printed to and a constant
 *   refrence to the Rule or Item to be printed.
 * Effect: Characters sent to stream buffer.
 * Return: A reference to the stream printed to.
 * Format: Prints lhs " ->" followed by " " rhs[n] for n=0...cr().
 *   For Item is n= place " * " rhs[n] is printed instead.
 */

#endif//RULE_ITEM_HPP
