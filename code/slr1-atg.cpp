#include "slr1-atg.hpp"

// Implementation of Slr1Atg
// The access functions and the massive calculations.

#include "states.hpp"

// Helper Structure SymbolData Constructor ===================================
// Fills in the default values for a given symbol.
Slr1Atg::SymbolData::SymbolData (SymbolT sym) :
  nullable(false), first(), follow()
{
  // Terminals are there own first set.
  if (isTerminal(sym))
    first.insert(sym);
  // ? The starting symbol has the eof in its follow set.
}

// Constructors and Deconstructor ============================================
// Create a basic Slr1Atg for the given CFG.
Slr1Atg::Slr1Atg (CFGrammer cfg) :
  data(), grammer(cfg), symbols()
{
  // Initialize the symbols map.
  for (SymbolT isym = SymbolEnum::variable ; isym < cap ; ++isym)
    symbols.insert(std::make_pair(isym, SymbolData(isym));

  // Calculate the actual values for all symbols.
  preformAllCalc();

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

Slr1Atg::~Slr1Atg ()
{}

// The Three Calculator Functions=============================================
// Rule -> Boolean * rule to proccess -> did that change anything
// Also they have to be run in order.

bool Slr1Atg::calcNullable (Rule rule)
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

bool Slr1Atg::calcFirst (Rule rule)
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
      hasChanged = firstSet.insert(*it).second || hasChanged;

    // Break on non-null rhs symbol
    if (!isNullable(rule.rhs[i])) break;
  }

  return hasChanged;
}

bool Slr1Atg::calcFollow (Rule rule)
{
  // Keep a look out for any changes.
  bool hasChanged = false;

  // The set of symbols to add, starts with the follow of the rhs.
  std::set<SymbolT> addSet = symbols[rule.lhs].follow;

  // Iterators: because their declarations are too long.
  std::vector<SymbolT>::reverse_iterator it;
  std::set<SymbolT>::iterator jt;

  // For each symbol on the rhs ...
  for (it = rule.rhs.rbegin() ; it != rule.rhs.rend() ; ++it))
  {
    // ... get its follow set ...
    std::set<SymbolT> & followSet = symbols[*it].follow;

    // ... union it with the addSet ...
    for (jt = addSet.begin() ; jt != addSet.end() ; ++jt)
      hasChanged = followSet.insert(*jt).second || hasChanged;

    // ... clear the addSet if the symbol is non-nullable ...
    if (!isNullable(*it))
      addSet.clear();

    // ... and add the symbol's first set to the addSet.
    for (jt = symbols[*it].first.begin() ;
         jt != symbols[*it].first.end() ; ++jt)
      addSet.insert(*jt);
  }

  return hasChanged;
}

// Callculate the SROps using nullable, first & follow -----------------------
void calcOperations ()
{
  // This is not simply, infact I use a state machine to make it work.
  StateMachine<StateT, SymbolT, std::vector<Item> > states;
  /* Each state is the set of Items we could be proccessing.
   * Transitions out of a state repersent shift operations that would
   * progress an Item (or Items) in the state. The starting state starts with
   * just the fresh Item of the starting symbols rules (?).
   *
   * After a new State is created for all Rules that are about to read in a
   * non-terminal symbol the fresh Item for every rule with that symbol on the
   * lhs. Recusively if needed.
   *
   * Actually this part, generating the StateGraph, might be worth taking out.
   */

  // Keep a counter, that holds the value of the next state we would make
  // if we need to make a new on. Update with increment.
  StateT nextState = 0;

  /* After the state graph has been generated the graph can be used to define
   * most (all?) the shift operations, one for each transition in the graph.
   * Reduce operations require the follow sets. Each time a "Full Item" is
   * encountered in a state a reduce operation is defined from that state, by
   * each symbol in the Item's lhs, for the Item's base Rule.
   */
}

// ---------------------------------------------------------------------------
/* Call all of the calculation functions, in order and repeating each until
 *   they are stable and no more updates have to be made.
 */
void Slr1Atg::preformAllCalc ()
{
#define REPEAT_OVER_RULES(calcFunc) \
  do { \
    bool change = false; \
    for (std::set<Rule>::iterator it = grammer.rules.begin() ; \
         it != grammer.rules.end() ; ++it) \
      change =|| calcFunc(*it); \
  } while (change);

  REPEAT_OVER_RULES(calcNullable)
  REPEAT_OVER_RULES(calcFirst)
  REPEAT_OVER_RULES(calcFollow)

#undef REPEAT_OVER_RULES
}

// Implementation Functions ==================================================
// Check to see if a SymbolT can expand to nothing (an empty series).
bool Slr1Atg::isNullable (SymbolT sym) const
{ return symbols[sym].nullable; }

// Get the set of symbols that can be first in an expantion of a SymbolT.
std::set<SymbolT> Slr1Atg::firstSet (SymbolT sym) const
{ return symbols[sym].first; }

// Get the set of symbols that can be follow the expanition of a SymbolT.
std::set<SymbolT> Slr1Atg::followSet (SymbolT sym) const
{ return symbols[sym].follow; }

// Create and return a new ActionTable from this generator.
ActionTable Slr1Atg::generate () const
{
  // Check to make sure everything is OK.
  if (!canGenerate())
  {
    printProblems(std::cerr);
    throw std::runtime_error(
        "Slr1Atg: generate() called while !canGenerate()");
  }

  // Iterator declarations:
  std::map<StateT, std::map<SymbolT, std::vector<SROp> > >::iterator stateIT;
  std::map<SymbolT, std::vector<SROp> >::iterator symbolIT;

  // Now the actual generation.
  ActionTable actTab;

  // For every state in data:
  for (stateIT = data.begin() ; stateIT != data.end() ; ++stateIT)
    // For every transition out of every state
      for (symbolIT = stateIT->second.begin() ;
           symbolIT != stateIT->second.end() ; ++symbolIT)
        // Is there is exactly 1 possible destination
        // (There should never be more than 1.)
        if (1 == symbolIT->second.size())
          // Copy it into the ActionTable.
          actTab.setOp(*stateIT, *symbolIT, symbolIT->second[0])
  return actTab;
}

// Check to see if the instance can currantly generate an ActionTable.
bool Slr1Atg::canGenerate () const
{
  // Iterator declarations:
  std::map<StateT, std::map<SymbolT, std::vector<SROp> > >::iterator stateIT;
  std::map<SymbolT, std::vector<SROp> >::iterator symbolIT;

  // For every state in data:
  for (stateIT = data.begin() ; stateIT != data.end() ; ++stateIT)
    // For every transition out of every state
      for (symbolIT = stateIT->second.begin() ;
           symbolIT != stateIT->second.end() ; ++symbolIT)
        // Is there than 1 possible destination?
        if (1 < symbolIT->second.size())
          // If so we can't generate an ActionTable.
          return false;
  // If everything matches, we can produce a generator.
  return true;
}

// Print the problems that prevent a ActionTable from being generated.
void Slr1Atg::printProblems (std::ostream & out) const
{
  // Iterator declarations:
  std::map<StateT, std::map<SymbolT, std::vector<SROp> > >::iterator stateIT;
  std::map<SymbolT, std::vector<SROp> >::iterator symbolIT;

  // For every state in data:
  for (stateIT = data.begin() ; stateIT != data.end() ; ++stateIT)
    // For every transition out of every state
      for (symbolIT = stateIT->second.begin() ;
           symbolIT != stateIT->second.end() ; ++symbolIT)
        // Is more than 1 possible destination?
        if (1 < symbolIT->second.size())
        {
          // Print conflict:
          out << '(' << *stateIT << ", " << *symbolIT << ')' <<
              " has " << symbolIT->second.size() << " possible outcomes."
              << std::endl;
        }
}
