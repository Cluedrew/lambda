#include "cfgrammer.hpp"

// Implementation of defineCFGrammer()

#include <string>
#include <sstream>
#include <queue>
#include "symbol.hpp"

// Helper Functions ==========================================================

// List of whitespace characters.
static std::string whitespace = std::string(" \t\n\r");

// Get the next non-empty non-commented line from input.
// This is used to break up lines
static std::istringstream & getGoodLine (std::istream & in)
{
  // Unfortunatly I can't seem to return stringstreams. To get around that
  // I'm returning a refence to a perminate stream.
  // The contents are reset every call to getGoodLine()
  static std::istringstream iss;

  // Set the stream's connents.
  std::string line;
  do {
    getline(in, line);
  }
  while (line.find_first_not_of(whitespace) == line.npos);
  iss.str(line);
  return iss;
}

// Convert a character stream into a set.
template<typename T>
static std::set<T> streamToSet
    (T (*mapping)(std::istringstream &), std::istream & in)
{
  // Get the size of the set.
  std::istringstream & tmp = getGoodLine(in);
  int n;
  tmp >> n;

  // Create the set to return.
  std::set<T> fin;

  // Insert all the elements into the set.
  for (int i = 0 ; i < n ; ++i)
    fin.insert(mapping(getGoodLine(in)));

  return fin;
}

// Convert a character stream into a verctor.
template<typename T>
static std::vector<T> streamToVector
    (T (*mapping)(std::istringstream &), std::istream & in)
{
  // Get the size of the vector.
  std::istringstream & tmp = getGoodLine(in);
  int n;
  tmp >> n;

  // Create the set to return.
  std::vector<T> fin;

  // Insert all the elements into the set.
  for (int i = 0 ; i < n ; ++i)
    fin.push_back(mapping(getGoodLine(in)));

  return fin;
}

// Mapping Funtions ==========================================================
// Get a single symbol from the front of the stream and discard the rest.
static SymbolT lineToSymbolT (std::istringstream & ss)
{
  SymbolT fin;
  ss >> fin;
  return fin;
}

// Read in symbols from the line, the first is the lhs, the rest the rhs.
static Rule lineToRule (std::istringstream & ss)
{
  Rule rule;
  ss >> rule.lhs;
  SymbolT sym;
  while (ss >> sym)
    rule.rhs.push_back(sym);
  return rule;
}

// Make a CFG ================================================================
// Create a CFGrammer from a text definition.
CFGrammer CFGrammer::defineFromText (std::istream & in)
{
  CFGrammer fin;

  // Terminal Symbols
  fin.terminals = streamToSet<SymbolT>(lineToSymbolT, in);

  // NonTerminal Symbols
  fin.nonTerminals = streamToSet<SymbolT>(lineToSymbolT, in);

  // Start Symbol
  fin.start = lineToSymbolT(getGoodLine(in));

  // Production Rules
  fin.rules = streamToVector<Rule>(lineToRule, in);

  return fin;
}

// Iterators =================================================================
// A thought experament: going through rules for something that matches the
// for rules that match the left hand side is common.
// (I ended up trying to make a general version called the filter-iterator.)
MatchLeftIterator::MatchLeftIterator(SymbolT match, CFGrammer grammer) :
  match(match), at(grammer.rules.begin()), end(grammer.rules.end())
{
  while (at != end && at->lhs != match) ++at;
}

bool MatchLeftIterator::isNotEnd ()
{ return at != end; }

MatchLeftIterator & MatchLeftIterator::operator++ ()
{
  do {++at;} while (at != end && at->lhs != match);
  return *this;
}

MatchLeftIterator MatchLeftIterator::operator++ (int)
{
  MatchLeftIterator tmp = *this;
  do {++at;} while (at != end && at->lhs != match);
  return tmp;
}

Rule & MatchLeftIterator::operator* ()
{ return at.operator*(); }
Rule * MatchLeftIterator::operator-> ()
{ return at.operator->(); }

/*
What would a 'prefix free' programming language look like.

Define a subset of the symbols to be final symbols.
- The starting symbol must be a final symbol.
- No final symbol may appear anywhere in any production rule except if:
  - The lhs is a non-terminal final symbol.
  - The last symbol on the rhs is a final symbol (terminal or nonterminal).
  - No other symbols in the rule are final symbols.

Note: this is not a true definition of prefex free, but it is possible to
check for.
*/
bool checkPrefexFree (CFGrammer cfg)
{
  // The set of final symbols:
  std::set<SymbolT> final;

  // A queue of final symbols to be procced.
  std::queue<SymbolT> waiting;

  // The starting symbol must be a final symbol:
  waiting.push(cfg.start);

  // Loop through all the final symbols.
  while (!waiting.empty())
  {
    // Get the next symbol in the waiting queue.
    SymbolT currant = waiting.front();
    waiting.pop();

    // Iterate across all Rules in the language.
    for (std::vector<Rule>::const_iterator rit = cfg.rules.cbegin() ;
         rit != cfg.rules.cend() ; ++rit)
    {
      // If a rule's lhs matches...
      if (currant == rit->lhs)
      {
        // Than it must have a rhs that ends in a final symbol
        if (0 == rit->cr())
          return false;
        // If it is already a final symbol nothing else to do.
        else if (final.count(rit->rhs.back()))
          continue;
        // Otherwise add it to the set and the queue.
        else
        {
          final.insert(rit->rhs.back());
          waiting.push(rit->rhs.back());
        }
      }
    }
  }

  // That builds a set of all final symbols, now make sure they never appear
  // in the middle of of Rule. Go through all rules.
  for (std::vector<Rule>::const_iterator rit = cfg.rules.cbegin() ;
       rit != cfg.rules.cend() ; ++rit)
  {
    // And all middle places in the rules.
    int n = rit->cr() - 1;
    for (int i = 0 ; i < n ; ++i)
    {
      // If the symbol is a final symbol, the set is not prefex free.
      if (final.count(rit->rhs[i])) return false;
    }
  }
  // If no such symbol is found, the set is prefex free.
  return true;
}
