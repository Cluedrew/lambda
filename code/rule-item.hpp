#ifndef RULE_ITEM_HPP
#define RULE_ITEM_HPP

#include <iosfwd>

// The production rule and the items.
struct Rule
{
  SymbolT lhs;
  std::vector<SymbolT> rhs;
  unsigned int cr () const; // count right, rhs.size()
};

struct Item // ??? : public Rule
{
  SymbolT lhs;
  std::vector<SymbolT> rhs;
  unsigned int place; // In the range of [0..cr()]
  unsigned int cr () const; // count right, rhs.size()
};

/*
std::ostream & operator<< (std::ostream &, Rule const &);
std::ostream & operator<< (std::ostream &, Item const &);
/ * Print the Rule or Item to the given stream.
 * Params: A reference to the stream to be printed to and a constant
 *   refrence to the Rule or Item to be printed.
 * Effect: Characters sent to stream buffer.
 * Return: A reference to the stream printed to.
 * Format: Prints lhs " ->" followed by " " rhs[n] for n=0...cr().
 *   For Item is n= place " * " rhs[n] is printed instead.
 * On hold until I have a print operation for the symbol_t.
 */

#endif//RULE_ITEM_HPP
