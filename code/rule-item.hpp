#ifndef RULE_ITEM_HPP
#define RULE_ITEM_HPP

// The production Rule structure and the Item there of.
// Not that the constrants on Item are not enforced.

// These need some cleaning, including a switch from
// LHS -> RHS[..] to LHS ( RHS[..] ) or something to avoid the \n delimiter.

// A reminder to change over the format.
#define RI_IO_FORMATE "arrow"

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
