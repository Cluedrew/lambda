#include "slr1-atg.hpp"

// Implementation of Slr1Atg
// The access functions and the massive calculations.

#include <iostream>
#include "symbol.hpp"

// Constructors and Deconstructor ============================================
// Create a basic Slr1Atg for the given CFG.
Slr1Atg::Slr1Atg (CFGrammer cfg) :
  grammer(cfg), data(), symbols(), stateGraph()
{
  // Initialize the symbols map.
  for (SymbolT isym = SymbolEnum::variable ; isym < SymbolEnum::cap ; ++isym)
    symbols.insert(std::make_pair(isym, SymbolData(isym)));

  // Includes added the eof to the start symbol's follow set.
  // This is because of the imaginary rule: -> S eof.
  symbols[grammer.start].follow.insert(getEofSymbol());

  // Calculate the fill in all the SymbolData for all symbols.
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

// ===========================================================================
// Helper Structure SymbolData Constructor -----------------------------------
// Fills in the default vaules for a generic symbol.
Slr1Atg::SymbolData::SymbolData () :
  nullable(false), first(), follow()
{}

// Fills in the default values for a given symbol.
Slr1Atg::SymbolData::SymbolData (SymbolT sym) :
  nullable(false), first(), follow()
{
  // Terminals are their own first set.
  if (isTerminal(sym))
    first.insert(sym);
}

// ===========================================================================
// The Three SymbolData Calculator Functions ---------------------------------
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
    if (!isNullable(rule.rhs[i]))
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
    for (std::set<SymbolT>::const_iterator it = innerSet.cbegin() ;
         it != innerSet.cend() ; ++it)
      // ... try adding it to the this first set and check the result.
      hasChanged = firstSet.insert(*it).second || hasChanged;

    // Break on non-null rhs symbol.
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
  for (it = rule.rhs.rbegin() ; it != rule.rhs.rend() ; ++it)
  {
    // ... get its follow set ...
    std::set<SymbolT> & followSet = symbols[*it].follow;

    // ... union (assign) the follow set with the addSet ...
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

// ===========================================================================
// Fill a state from its kurnal to a complete state.
void Slr1Atg::fillState (LabelT & state)
{
  // For each Item in the state ...
  for (unsigned int i = 0 ; i < state.size() ; ++i)
  {
    Item & item = state[i];
    // ... if there is a next symbol for the item...
    if (item.cr() == item.place) continue;
    // ... and the next symbol is a nonTerminal.
    SymbolT curSym = item.rhs[item.place];
    if (isNonTerminal(curSym))
    {
      // Then add a fresh item for each Rule with
      //   the currant symbol as the lhs.
      for (MatchLeftIterator it(curSym, grammer) ; it.isNotEnd() ; ++it)
      {
        // Get the fresh item.
        Item const freshItem = it->getFresh();
        // Make sure this is not a repeat.
        bool unique = true;
        for (std::vector<Item>::const_iterator jt = state.cbegin() ;
            jt != state.cend() ; ++jt)
          if (freshItem == *jt)
            { unique = false; break; }
        // If this is unique for this state at it to the state.
        if (unique) state.push_back(freshItem);
      }
    }
  }
}

// Find the subset of a state's items that can be advanced/shifted with sym.
Slr1Atg::LabelT Slr1Atg::shiftGroup (LabelT const & label, SymbolT sym)
{
  LabelT fin;
  for (LabelT::const_iterator it = label.begin() ; it != label.end() ; ++it)
    if (sym == it->rhs[it->place])
      fin.push_back(*it);
  return fin;
}

/*   From an existing state find the state that would be found at the end of
 * an edge labeled sym. If no such state exists (aka, it would be empty and
 * never be reached while parsing) the edge does not exist. If the state does
 * exist (logically) it will be created if it is not in the graph and an edge
 * is set to connect them.
 *   This overwrites an existing edge/transition from state labeled sym.
 * Params: state: The state to start from.
 *         sym: The symbol the edge is/would be labeled with.
 * Return: (false, ?) where ? is a garbage value if the dest-state does not
 *   exist, if the state does exist returns (true, id) where id is the
 *   dest-state's id.
 */
std::pair<bool, StateT> Slr1Atg::destState (StateT state, SymbolT sym)
{
  // Find the kernal of the new state
  LabelT nLabel = shiftGroup(stateGraph.lookUp(state), sym);
  // If empty delete any existing edge and stop.
  if (nLabel.empty())
  {
    stateGraph.delTrans(state, sym);
    return std::make_pair(false, 0);
  }
  // Otherwise expand the label.
  fillState(nLabel);
  // Get the id of the state with that label, wheither new or old.
  StateT id = stateGraph.addState(nLabel).second;
  // Connect them (check for existing transition?)
  stateGraph.setTrans(state, sym, id);
  return std::make_pair(true, id);
}

// Create a StateGraph that shows possible states of the stack and what
// parse rules can be ongoing at that time.
void Slr1Atg::calcStateGraph ()
{
  // Fill in the stateGraph.

  // Possibly insert some code to drain the graph so this doesn't break
  // everything if it is called twice.

  /* ===== Create the Imaginary Production Rule =====
   *   -> S eof : The imaginary rule; not part of the grammer (although eof is
   * added to S's (the start symbol's) follow set as if it was. It gets rid
   * of special cases at the end of the stream and the top of the parse tree.
   *   Its fresh item -> * S eof is the kernal of the starting state and the
   * second item -> S * eof appears in the end state. Because eof is never
   * shifted the third item never appears.
   *   Implementation detail: eof is used as the lhs, but the rule is never
   * used in symbol property calculations, so eof isn't going to end up with a
   * first set.
   */
  Rule imaginaryRule;
  {
    std::vector<SymbolT> tmp_rhs;
    tmp_rhs.push_back(grammer.start);
    tmp_rhs.push_back(getEofSymbol());
    imaginaryRule.rhs = tmp_rhs;
    imaginaryRule.lhs = getEofSymbol();
  }
  Item imaginaryItem1 = imaginaryRule.getFresh();

  // ===== Beginning and End State Set Up =====
  // The trick is to set up these special cases so they don't conflict
  // with the regular generation.
  // Rule imaginaryRule {getEofSymbol(), {grammer.start, getEofSymbol()}}

  // Set up the starting state. (state 0)
  // Use the imaginaryRule as the kurnal, fill out the state.
  StateT startState;
  {
    LabelT startLabel = std::vector<Item>(1, imaginaryRule.getFresh());
    fillState(startLabel);
    stateGraph.setStart(
        (startState = stateGraph.addState(startLabel).second) );
  }

  // Set up the end state. (state 1)
  // Regular advancement on S from starting state, add the done operation
  // to the state on eof in look-ahead. (I am confident this will not fail.)
  StateT endState = destState(startState, grammer.start).second;
  data[std::make_pair(endState, getEofSymbol())].push_back(SROp::doneOp());

  // ===== Intermediate State Set Up =====
  // Loop to create the remaining states.
  // Process each state by progressing items by one.
  for (StateT proc = 0 ; stateGraph.isState(proc) ; ++proc)
  {
    // For each symbol, find the destState.
    // Would it be faster to go by Rules or Symbols?
  }

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
}

// Callculate the SROps using nullable, first & follow -----------------------
void calcOperations ()
{
  /* After the state graph has been generated the graph can be used to define
   * most (all?) the shift operations, one for each transition in the graph.
   * Reduce operations require the follow sets. Each time a "Full Item" is
   * encountered in a state a reduce operation is defined from that state, by
   * each symbol in the Item's lhs, for the Item's base Rule.
   */
}

// ===========================================================================
/* Call all of the calculation functions, in order and repeating each until
 *   they are stable and no more updates have to be made.
 */
void Slr1Atg::preformAllCalc ()
{
  bool change;
#define REPEAT_OVER_RULES(calcFunc) \
  do { \
    change = false; \
    for (std::vector<Rule>::const_iterator it = grammer.rules.begin() ; \
         it != grammer.rules.end() ; ++it) \
      change = calcFunc(*it) || change; \
  } while (change);

  REPEAT_OVER_RULES(calcNullable)
  REPEAT_OVER_RULES(calcFirst)
  REPEAT_OVER_RULES(calcFollow)

#undef REPEAT_OVER_RULES
}

// Implementation Functions ==================================================
// Check to see if a SymbolT can expand to nothing (an empty series).
bool Slr1Atg::isNullable (SymbolT sym) const
{ return symbols.find(sym)->second.nullable; }

// Get the set of symbols that can be first in an expantion of a SymbolT.
std::set<SymbolT> Slr1Atg::firstSet (SymbolT sym) const
{ return symbols.find(sym)->second.first; }

// Get the set of symbols that can be follow the expanition of a SymbolT.
std::set<SymbolT> Slr1Atg::followSet (SymbolT sym) const
{ return symbols.find(sym)->second.follow; }

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
  std::map<std::pair<StateT, SymbolT>, std::vector<SROp> >::const_iterator it;

  // Now the actual generation.
  ActionTable actTab;

  // For every state symbol combination:
  for (it = data.begin() ; it != data.end() ; ++it)
    // If an operation is defined save it (2 or more has been checked for)
    if (1 == it->second.size())
      actTab.setOp(it->first.first, it->first.second, it->second[0]);
  return actTab;
}

// Check to see if the instance can currantly generate an ActionTable.
bool Slr1Atg::canGenerate () const
{
  // Iterator declarations:
  std::map<std::pair<StateT, SymbolT>, std::vector<SROp> >::const_iterator it;

  // For every state symbol combination:
  for (it = data.begin() ; it != data.end() ; ++it)
    // Make sure there aren't multiple operations defined.
    if (1 < it->second.size())
      return false;
  // If there are no conficts, an action table can be generated.
  return true;
}

// Print the problems that prevent a ActionTable from being generated.
void Slr1Atg::printProblems (std::ostream & out) const
{
  // Iterator declarations:
  std::map<std::pair<StateT, SymbolT>, std::vector<SROp> >::const_iterator it;

  // For every state symbol combination:
  for (it = data.begin() ; it != data.end() ; ++it)
    // Make sure there aren't multiple operations defined.
    if (1 < it->second.size())
    {
      // Print conflict:
      out << '(' << it->first.first << ", " << it->first.second << ") has " <<
          it->second.size() << " possible outcomes." << std::endl;
    }
}

// LabelTEquals Definition ===================================================
bool Slr1Atg::LabelTEquals::operator() (LabelT const & lhs,
                                        LabelT const & rhs) const
{
  // The labels must have the same set of items.
  if (lhs.size() == rhs.size())
  {
    LabelT::const_iterator lit = lhs.cbegin();
    LabelT::const_iterator rit = rhs.cbegin();
    for (; lit != lhs.cend() ; ++lit, ++rit)
      if (*lit != *rit)
        return false;
    return true;
  }
  // If there are different numbers of Items the labels are not equal.
  else
    return false;
}
