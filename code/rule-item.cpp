#include "rule-item.hpp"

// Implementation of cr() and << for Rule and Item... and so much more now.

#include <ostream>
#include <istream>
#include <sstream>
#include "symbol.hpp"

// Constructors and <s>Deconstructor</s>
Rule::Rule(SymbolT lhs, std::vector<SymbolT> rhs) :
    lhs(lhs), rhs(rhs)
{}

Rule::Rule() :
    lhs(), rhs(0)
{}

Item::Item(SymbolT lhs, std::vector<SymbolT> rhs, unsigned int place) :
    lhs(lhs), rhs(rhs), place(place)
{}

Item::Item() :
    lhs(), rhs(0), place(0)
{}

// count right, rhs.size()
unsigned int Rule::cr () const
{
  return rhs.size();
}
unsigned int Item::cr () const
{
  return rhs.size();
}



// Comparison Predicates =====================================================

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
  return !(*this == other);
}

bool Item::operator!= (Item const & other) const
{
  return !(*this == other);
}

// Conversion Functions ======================================================
// Get a fresh Item from a Rule
Item Rule::getFresh () const
{
  return Item(lhs, rhs, 0);
}

// Get the base Rule for this Item.
Rule Item::getBase () const
{
  return Rule(lhs, rhs);
}

// Get a new Item that is this Item progressed by one.
Item Item::getNext () const
{
  return Item(lhs, rhs, place + 1);
}



// I/O =======================================================================

// Print the Rule to the given stream.
std::ostream & operator<< (std::ostream & out, Rule const & rule)
{
  // lhs and opening bracket.
  out << rule.lhs << " (";

  // All rhs symbols.
  for (std::vector<SymbolT>::const_iterator it = rule.rhs.cbegin()
       ; it != rule.rhs.cend() ; ++it)
  {
    out << " " << *it;
  }

  // Closing bracket and finish.
  return (out << " )");
}

// Print the Item to the given stream.
std::ostream & operator<< (std::ostream & out, Item const & item)
{
  // lhs and opening bracket.
  out << item.lhs << " (";

  // rhs symbols before the *.
  size_t index = 0;
  for ( ; index < item.place && item.place <= item.cr() ; ++index)
  {
    out << " " << item.rhs[index];
  }

  // The * to repersent place.
  out << " *";

  // The remaining rhs symbols.
  for ( ; index < item.cr() ; ++index)
  {
    out << " " << item.rhs[index];
  }

  // Closing bracket and end.
  return (out << " )");
}

#define IN_ABORT if(in.bad()) return in;

// Read a Rule from a stream.
std::istream & operator>> (std::istream & in, Rule & rule)
{
  Rule tmp;

  // Get lhs & opening bracket.
  in >> tmp.lhs;
  IN_ABORT
  char chCatch;
  in >> chCatch;
  IN_ABORT

  std::string str;
  do {
    // Read the next word in.
    in >> str;
    IN_ABORT

    // Check for a special symbol.
    if (str == ")")
      break;
    // Add a symbol to the rhs.
    else
      tmp.rhs.push_back(stringToSymbol(str));

  } while (str != ")");

  rule = tmp;
  return in;
}

// Read an Item from a stream.
std::istream & operator>> (std::istream & in, Item & item)
{
  Item tmp;

  // Get lhs & opening bracket.
  in >> tmp.lhs;
  IN_ABORT
  char chCatch;
  in >> chCatch;
  IN_ABORT

  // Track how many symbols we have read in.
  size_t curI = 0;

  std::string str;
  do {
    // Read the next word in.
    in >> str;
    IN_ABORT

    // Check for a special symbol.
    if (str == ")")
      break;
    else if (str == "*")
      tmp.place = curI;
    // Add a symbol to the rhs.
    else
      tmp.rhs.push_back(stringToSymbol(str));

    ++curI;
  } while (str != ")");

  item = tmp;
  return in;
}

#undef IN_ABORT
