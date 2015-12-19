#include "rule-item.hpp"

// Implementation of cr() and << for Rule and Item

#include <ostream>
#include "symbol.hpp"



// count right, rhs.size()
unsigned int Rule::cr () const
{
  return rhs.size();
}
unsigned int Item::cr () const
{
  return rhs.size();
}

// Less than comparison
bool Rule::operator< (Rule const & other) const
{
  // Left hand side takes presidence
  if (lhs < other.lhs) return true;
  else if (lhs > other.lhs) return false;
  // Right hand side come next
  else if (rhs < other.rhs) return true;
  // Equal to or greator than
  else return false;
}
bool Item::operator< (Item const & other) const
{
  // Left hand side takes presidence
  if (lhs < other.lhs) return true;
  else if (lhs > other.lhs) return false;
  // Right hand side come next
  else if (rhs < other.rhs) return true;
  else if (rhs > other.rhs) return false;
  // Place acts as another tie breaker
  else if (place < other.place) return true;
  // Equal to or greator than
  else return false;
}

// Equality Comparison: All fields must match.
bool Rule::operator== (Rule const & other) const
{
  if (lhs == other.lhs && cr() == other.cr())
  {
    for (unsigned int i = 0 ; i < cr() ; ++i)
      if (rhs[i] != other.rhs[i])
        return false;
    return true;
  }
  return false;
}
bool Item::operator== (Item const & other) const
{
  if (lhs == other.lhs && cr() == other.cr() && place == other.place)
  {
    for (unsigned int i = 0 ; i < cr() ; ++i)
      if (rhs[i] != other.rhs[i])
        return false;
    return true;
  }
  return false;
}

// Inequality Comparison: Any field must miss-match.
bool Rule::operator!= (Rule const & other) const
{
  if (lhs == other.lhs && cr() == other.cr())
  {
    for (unsigned int i = 0 ; i < cr() ; ++i)
      if (rhs[i] != other.rhs[i])
        return true;
    return false;
  }
  return true;
}
bool Item::operator!= (Item const & other) const
{
  if (lhs == other.lhs && cr() == other.cr() && place == other.place)
  {
    for (unsigned int i = 0 ; i < cr() ; ++i)
      if (rhs[i] != other.rhs[i])
        return true;
    return false;
  }
  return true;
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

// Get a fresh Item from a Rule
Item Rule::getFresh ()
{
  return Item{lhs, rhs, 0};
}

// Get the base Rule for this Item.
Rule Item::getBase ()
{
  return Rule{lhs, rhs};
}

// Get a new Item that is this Item progressed by one.
Item Item::getNext ()
{
  return Item{lhs, rhs, place + 1};
}
