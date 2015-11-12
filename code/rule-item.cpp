#include "rule-item.hpp"

// Implementation of cr() and << for Rule and Item

// count right, rhs.size()
size_type Rule::cr ()
{
  return rhs.size();
}

// count right
size_type Item::cr ()
{
  return rhs.size();
}

#if 0
// if SymbolT::operator<< is defined.

// Print the Rule or Item to the given stream.
std::ostream & operator<< (std::ostream & out, Rule const & rule)
{
  out << rule.lhs << " ->";
  for (unsigned int i = 0 ; i < rule.cr() ; ++i)
    out << " " << rule.rhs[i];
  return out;
};

std::ostream & operator<< (std::ostream & out, Item const & item)
{
  //if (item.cr() < item.place) error.

  out << item.lhs << " ->";
  unsigned int i = 0;
  for ( ; i < item.place ; ++i)
    out << " " << item.rhs[i];
  out << " *";
  for ( ; i < item.cr() ; ++i)
    out << " " << item.rhs[i];
  return out;
};

#endif
