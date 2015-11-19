#include "action-table.hpp"

// Implementation of the ActionTable and ActionTableGenerator.

#include <stdexcept>
#include <iostream>
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
  return data.count(state) && data.find(state)->second.count(symbol);
}

// Get the SROp for a given state and transition symbol.
SROp ActionTable::getOp (StateT state, SymbolT symbol) const
{
  // Check to make sure one is defined, return it if it exists.
  if (!hasOp(state, symbol))
    throw std::invalid_argument("ActionTable: cannot getOp for undefined Op");
  return data.find(state)->second.find(symbol)->second;
}

// Set the operation for a given state and lookahead symbol.
void ActionTable::setOp (StateT state, SymbolT symbol, SROp sr)
{
  // Is there a map for the state?
  if (data.count(state))
  {
    // Is there a mapped value for the symbol?
    if (data[state].count(symbol))
      // If so we can overwrite the old one.
      data[state][symbol] = sr;
    else
      // If not create a new one.
      data[state].insert(std::pair<SymbolT, SROp>(symbol, sr));
  }
  else
  {
    // Make a map for the state and insert the (symbol -> sr) pair.
    std::map<SymbolT, SROp> tmp;
    tmp.insert(std::pair<SymbolT, SROp>(symbol, sr));
    data.insert(std::pair<StateT, std::map<SymbolT, SROp> >(state, tmp));
  }
}

// Remove an operation from the ActionTable.
void ActionTable::delOp (StateT state, SymbolT symbol)
{
  // If the state's map doesn't exist there is nothing to delete.
  if (data.count(state))
  {
    // If the symbol is defined in the map then we have to delete it.
    if (data[state].count(symbol))
    {
      data[state].erase(symbol);
      // If that was the last symbol in this map, remove the state map.
      if (data[state].empty())
        data.erase(state);
    }
  }
}
