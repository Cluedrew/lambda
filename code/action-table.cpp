#include "action-table.hpp"

// Implementation of the ActionTable and ActionTableGenerator.

#include "symbol.hpp"

// ===========================================================================
// ActionTable ===============================================================

// Constructors and Deconstructor ============================================
// Create an unfilled ActionTable.
ActionTable::ActionTable () :
  data()
{}

// Create and fill an ActionTable from a generator.
ActionTable::ActionTable (ActionTableGenerator const &);

ActionTable::~ActionTable ();

// Implementation Functions ==================================================
// Replace all SROps in the ActionTable with those from a generator.
ActionTable & ActionTable::operator= (ActionTableGenerator const & atg);

// Check to see if a given state and transition symbol has a defined SROp.
bool ActionTable::hasOp (StateT, SymbolT) const;

// Get the SROp for a given state
SROp ActionTable::getOp (StateT, SymbolT) const;

// Set the operation for a given state and lookahead symbol.
void ActionTable::setOp (StateT, SymbolT, SROp);

// Remove an operation from the ActionTable.
void ActionTable::delOp (StateT, SymbolT);

// ===========================================================================
// ActionTableGenerator ======================================================

// Helper Structure SymbolData Constructor
// Fills in the default values for a given symbol.
ActionTableGenerator::SymbolData::SymbolData (SymbolT sym) :
  nullable(false), first(), follow()
{
  // Terminals are there own first set.
  if (isTerminal(sym))
    first.insert(sym);
}

// Constructors and Deconstructor ============================================
// Create a basic ActionTableGenerator for the given CFG.
ActionTableGenerator::ActionTableGenerator (CFGrammer cfg) :
  data(), grammer(cfg), symbols()
{
  // Initialize the symbols map.
  for (SymbolT isym = SymbolEnum::variable ; isym < cap ; ++isym)
    symbols.insert(std::make_pair(isym, SymbolData(isym));

  // Calculate nullable
  // A symbol is nullable if it:
  // - Is the left hand side of a Rule with an empty right hand side.
  // - Is the left hand side of a Rule that entirely nullable symbols.
  // - Hence Terminals are never nullable.

  // Calculate first
  // The first set for a symbol S is:
  // - If S is Terminal: itself.
  // - If S is Nonterminal: the union of every first set of every symbol that
  //   appears in the rhs of a Rule with S as the lhs and ever symbol before
  //   the symbol on the rhs is nullable. (So always the first, the second if
  //   the first is nullable, the third if the first and the second are...)

  // Calculate follow
  // The follow set for a symbol S is:
  // - The union of first sets of symbols that follow S on the rhs of a Rule,
  //   either directly or seperated by nullable symbols.
  // - If S is the last symbol in a Rule's rhs (or all following symbols are
  //   nullable) than also union the follow set of the Rule's lhs symbol.
}

ActionTableGenerator::~ActionTableGenerator ()
{}

// The Three Calculator Functions=============================================
// Rule -> Boolean * rule to proccess -> did that change anything
// Also they have to be run in order.

bool CalcNullable (Rule rule)
{
  // We never need to re-check nullable symbols.
  if (isNullable(rule.lhs))
    return false;

  // Search for a non-nullable symbol in the rule,
  // if none is found than the rule is nullable (or null).
  bool mightNull = true;
  for (unsigned int i = 0 ; i < rule.cr() ; ++i)
    if (!isNullable(rule.rhs[i])
      { mightNull = false; break; }

  // If the rule was found to be nullable, the symbol is too.
  if (mightNull)
  {
    symbols[rule.lhs].nullable = true;
    return true;
  }
  else
    return false;
}

bool CalcFirst (Rule rule)
{
  // The set we might modify and whether it has changed.
  std::set<SymbolT> & firstSet = symbols[rule.lhs].first;
  bool hasChanged = false;

  // For each symbol in the rhs of the rule until we find a non-nullable one.
  for (unsigned int i = 0 ; i < rule.cr() ; ++i)
  {
    // Get the symbols first set
    std::set<SymbolT> & innerSet = symbols[rule.rhs[i]].first;
    // For each element in the symbols first set...
    for (std::set<SymbolT>::const_iterator it = innerSet.cbegin()) ;
         it != innerSet.cend() ; ++it)
      // ... try adding it to the this first set and check the result.
      firstSet.insert(*it).second || hasChanged;

    // Break on non-null rhs symbol
    if (!isNullable(rule.rhs[i])) break;
  }

  return hasChanged;
}

bool CalcFollow (Rule rule)
{

  // Keep a look out for any changes.
  bool hasChanged = false;

  // Maybe going backwards would be faster
  // actually I think I can cut a factor of cr() out of the execution time.
  // For each symbol on the rhs.
  for (i / it)
  {
    // The set we might modify.
    std::set<SymbolT> & mobSet = symbols[rule.rhs[*]].first;
    // For every following symbol
    for ( j / jt)
    {
    }
    // If we go off the end...
  }
}

// Implementation Functions ==================================================
// Check to see if a SymbolT can expand to nothing (an empty series).
bool ActionTableGenerator::isNullable (SymbolT sym) const
{ return symbols[sym].nullable; }

// Get the set of symbols that can be first in an expantion of a SymbolT.
std::set<SymbolT> ActionTableGenerator::firstSet (SymbolT sym) const
{ return symbols[sym].first; }

// Get the set of symbols that can be follow the expanition of a SymbolT.
std::set<SymbolT> ActionTableGenerator::followSet (SymbolT sym) const
{ return symbols[sym].follow; }

// Create and return a new ActionTable from this generator.
ActionTable ActionTableGenerator::generate () const;

// Check to see if the instance can currantly generate an ActionTable.
bool ActionTableGenerator::canGenerate () const;

// Print the problems that prevent a ActionTable from being generated.
void ActionTableGenerator::printProblems (std::ostream &) const;
