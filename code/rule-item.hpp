#ifndef RULE_ITEM_HPP
#define RULE_ITEM_HPP

// The production Rule structure and the Item there of.
// Not that the constrants on Item are not enforced.

// These need some cleaning, including a switch from
// LHS -> RHS[..] to LHS ( RHS[..] ) or something to avoid the \n delimiter.

#include <iosfwd>
#include <vector>
#include "parse-fwd.hpp"
struct Item;

// The production Rule
struct Rule
{
  Rule(SymbolT, std::vector<SymbolT>);
  Rule();
  SymbolT lhs;
  std::vector<SymbolT> rhs;
  unsigned int cr () const; // count right, rhs.size()
  bool operator< (Rule const &) const;
  bool operator== (Rule const &) const;
  bool operator!= (Rule const &) const;

  // Return a fresh item from this production Rule.
  Item getFresh () const;
};

// The Item
struct Item // : public Rule
{
  Item(SymbolT, std::vector<SymbolT>, unsigned int);
  Item();
  SymbolT lhs;
  std::vector<SymbolT> rhs;
  unsigned int place; // In the range of [0..cr()]
  unsigned int cr () const; // count right, rhs.size()
  bool operator< (Item const &) const;
  bool operator== (Item const &) const;
  bool operator!= (Item const &) const;

  // Return the base production Rule of the item.
  Rule getBase () const;
  // Return a new Item that is this item progressed by one.
  Item getNext () const;
};

std::ostream & operator<< (std::ostream &, Rule const &);
std::ostream & operator<< (std::ostream &, Item const &);
/* Print the Rule or Item to the given stream.
 * Params: A reference to the stream to be printed to and a constant
 *   refrence to the Rule or Item to be printed.
 * Effect: Characters sent to stream buffer.
 * Return: A reference to the stream printed to.
 * Format: Prints lhs " ->" followed by " " rhs[n] for n=0...cr().
 *   For Item is n= place " * " rhs[n] is printed instead. There is a
 *   tailing newline.
 */

std::istream & operator>> (std::istream &, Rule &);
std::istream & operator>> (std::istream &, Item &);
/* Read a Rule or Item from a stream.
 * Params: A reference to the stream to read from and a reference to
 *   the variable to store the Rule or Item in.
 * Effect: Stream is read from, value in the container will be updated and
 *   on error stream's badbit is set.
 * Return: A reference to the stream provided.
 * Format: LHS -> RHS[0] RHS[1] RHS[2] ...
 *   For items " * RHS[place]" replaces " RHS[place]". Both have a tailing
 *   newline that is read in.
 */

#endif//RULE_ITEM_HPP
