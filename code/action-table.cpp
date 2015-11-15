#include "action-table.hpp"

// Implementation of the ActionTable and ActionTableGenerator.

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

// Constructors and Deconstructor ============================================
// Create a basic ActionTableGenerator for the given CFG.
ActionTableGenerator::ActionTableGenerator (CFGrammer cfg) :
  data(), grammer(cfg), nullable(), follow(), first()
{
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

ActionTableGenerator::~ActionTableGenerator ();

// Implementation Functions ==================================================
// Check to see if a SymbolT can expand to nothing (an empty series).
bool ActionTableGenerator::isNullable (SymbolT sym) const
{ return nullable[sym]; }

// Get the set of symbols that can be first in an expantion of a SymbolT.
std::set<SymbolT> ActionTableGenerator::firstSet (SymbolT sym) const
{ return first[sym]; }

// Get the set of symbols that can be follow the expanition of a SymbolT.
std::set<SymbolT> ActionTableGenerator::followSet (SymbolT sym) const
{ return follow[sym]; }

// Create and return a new ActionTable from this generator.
ActionTable ActionTableGenerator::generate () const;

// Check to see if the instance can currantly generate an ActionTable.
bool ActionTableGenerator::canGenerate () const;

// Print the problems that prevent a ActionTable from being generated.
void ActionTableGenerator::printProblems (std::ostream &) const;
