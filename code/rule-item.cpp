#include "rule-item.hpp"

// Implementation of cr() and << for Rule and Item

#include <ostream>
#include "symbol.hpp"

// count right, rhs.size()
unsigned int Rule::cr () const
{
  return rhs.size();
}

// count right
unsigned int Item::cr () const
{
  return rhs.size();
}

// Print the Rule to the given stream.
std::ostream & operator<< (std::ostream & out, Rule const & rule)
{
  out << rule.lhs << " ->";
  for (unsigned int i = 0 ; i < rule.cr() ; ++i)
    out << " " << rule.rhs[i];
  return out;
}

// Print the Item to the given stream.
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
}
