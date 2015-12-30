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
#if RI_IO_FORMAT == arrow

// Print the Rule to the given stream.
std::ostream & operator<< (std::ostream & out, Rule const & rule)
{
  out << rule.lhs << " ->";
  for (unsigned int i = 0 ; i < rule.cr() ; ++i)
    out << " " << rule.rhs[i];
  return out << std::endl;
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
  return out << std::endl;
}

// Read a Rule from a stream.
std::istream & operator>> (std::istream & in, Rule & rule)
{
  // I sort of cheated, to keep from writing it out twice, read in an
  // item (with an invalid place field) then convert to a Rule.
  Item item;
  in >> item;
  rule = item.getBase();
  return in;
}

// Read an Item from a stream.
std::istream & operator>> (std::istream & in, Item & item)
{
  // Set up variables and read in the line.
  std::string str;
  getline(in, str);
  std::istringstream iss(str);
  SymbolT symbol;

  // Read in lhs.
  iss >> symbol;
  if (in.bad())
  {
    in.setstate(std::ios_base::badbit);
    return in;
  }
  else
    item.lhs = symbol;

  // Read in the "->".
  iss >> str;
  if (std::string("->") != str)
  {
    in.setstate(std::ios_base::badbit);
    return in;
  }

  // Read in rhs.
  for (int i = 0 ; iss.good() ; ++i)
  {
    iss >> str;
    if (std::string("*") == str)
      item.place = i--;
    else
    {
      item.rhs.push_back(stringToSymbol(str));
    }
  }
  return in;
}

#elif RI_IO_FORMAT == bracketed

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
  out << " *"

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
  in >> lhs;
  IN_ABORT
  in >> '(';
  IN_ABORT

  do {
    // Read the next word in.
    std::string str;
    in >> str;
    IN_ABORT

    // Check for a special symbol.
    if (str == ")")
    {}
    // Add a symbol to the rhs.
    else
      tmp.rhs.push_back(stringToSymbol(str));

  } while (str != ")")

  rule = tmp;
  return in;
}

// Read an Item from a stream.
std::istream & operator>> (std::istream & in, Item & item)
{
  Item tmp;

  // Get lhs & opening bracket.
  in >> lhs;
  IN_ABORT
  in >> '(';
  IN_ABORT

  // Track how many symbols we have read in.
  size_t curI = 0;

  do {
    // Read the next word in.
    std::string str;
    in >> str;
    IN_ABORT

    // Check for a special symbol.
    if (str == ")")
    {}
    else if (str == "*")
      tmp.place = curI;
    // Add a symbol to the rhs.
    else
      tmp.rhs.push_back(stringToSymbol(str));

    ++curI;
  } while (str != ")")

  item = tmp;
  return in;
}

#undef IN_ABORT

#endif
