#include "action-table.hpp"

// Implementation of the ActionTable and ActionTableGenerator.

#include <stdexcept>
#include <ios>
#include <iostream>
#include <string>
#include <sstream>
#include "symbol.hpp"

// ===========================================================================
// ActionTable ===============================================================

// Constructors and Deconstructor ============================================
// Create an unfilled ActionTable.
ActionTable::ActionTable () :
  data()
{}

// Create and fill an ActionTable from a generator.
ActionTable::ActionTable (ActionTableGenerator const & atg) :
  data()
{
  ActionTable other = atg.generate();
  data = other.data;
}

ActionTable::~ActionTable ()
{}

// Implementation Functions ==================================================
// Replace all SROps in the ActionTable with those from a generator.
ActionTable & ActionTable::operator= (ActionTableGenerator const & atg)
{
  ActionTable other = atg.generate();
  if (atg.canGenerate())
  {
    data.clear();
    data = other.data;
  }
  return *this;
}

// Check to see if a given state and transition symbol has a defined SROp.
bool ActionTable::hasOp (StateT state, SymbolT symbol) const
{
  return data.count(std::make_pair(state, symbol));
}

// Get the SROp for a given state and transition symbol.
SROp ActionTable::getOp (StateT state, SymbolT symbol) const
{
  // Check to make sure one is defined, return it if it exists.
  if (!hasOp(state, symbol))
    throw std::invalid_argument("ActionTable: cannot getOp for undefined Op");
  return data.find(std::make_pair(state, symbol))->second;
}

// Set the operation for a given state and lookahead symbol.
void ActionTable::setOp (StateT state, SymbolT symbol, SROp sr)
{
  // Make the index value.
  std::pair<StateT, SymbolT> index(state, symbol);
  // If the value is defined, change it.
  if (data.count(index))
    data[index] = sr;
  // Otherwise create a new one.
  else
    data.insert(std::make_pair(index, sr));
}

// Remove an operation from the ActionTable.
void ActionTable::delOp (StateT state, SymbolT symbol)
{
  // Make the index value.
  std::pair<StateT, SymbolT> index(state, symbol);
  // If an op is defined there, remove it.
  if (data.count(index)) data.erase(index);
}

// Input and Output Operator Overloads =======================================
// Print an ActionTable to a stream.
std::ostream & operator<< (std::ostream & out, ActionTable const & at)
{
  // Size line.
  out << at.data.size();
  // The n operation lines.
  for (std::map<std::pair<StateT, SymbolT>, SROp>::const_iterator
       it = at.data.cbegin() ; it != at.data.cend() ; ++it)
    out << '\n' << it->first.first << ' ' << it->first.second << ' '
        << it->second;
  return out << std::endl;
}

// Read an ActionTable from a stream.
std::istream & operator>> (std::istream & in, ActionTable & at)
{
  // Local Storage variables.
  StateT state;
  SymbolT symbol;
  SROp op;
  std::string line;
  std::istringstream iss;
  int n;

  // Get the size.
  getline(in, line);
  iss.str(line);
  iss >> n;

  // Get all the operations.
  for (int i = 0 ; i < n && in.good() ; ++i)
  {
    getline(in, line);
    iss.str(line);
    iss >> state >> symbol >> op;
    if (in.bad())
      break;
    else
      at.data.insert(std::make_pair(std::make_pair(state, symbol), op));
  }
  return in;
}
