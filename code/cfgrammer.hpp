#ifndef CFGRAMMER_HPP
#define CFGRAMMER_HPP

/* A context free grammer (CFG for short) which describes a context free
 * language (CFL).
 */

#include <set>
#include <vector>
#include <iosfwd>
#include "parse-fwd.hpp"
#include "rule-item.hpp"

// Main class
struct CFGrammer
{
  // The sets of all terminal and non-terminal symbols used in the cfg.
  std::set<SymbolT> nonTerminals;
  std::set<SymbolT> terminals;
  // A series of all production rules in the language.
  std::vector<Rule> rules;
  // The grammer's start symbol (non-terminal).
  SymbolT start;

  static CFGrammer defineFromText (std::istream & in);
  /* Create a CFGrammer from a text definition.
   * Params: A character stream that contains the language definition.
   * Effect: The stream is read from to the end of the CFG definition.
   * Return: A new CFGrammer.
   * Except: ??
   * Format: A grammer must be speified in the following way:
   *   ???
   */
};

// Iterator Class
// If you want to iterate through Rules with a given lhs, use this.
class MatchLeftIterator
{
private:
  SymbolT match;
  std::vector<Rule>::iterator at;
  std::vector<Rule>::iterator end;
protected:
public:
  MatchLeftIterator(SymbolT match, CFGrammer grammer);
  /* Create a new MatchLeftIterator for a match value and grammer.
   * Params: match is the symbol to be matched, when the iterator is
   *   incremented it will skip over Rules in the grammer that have a lhs
   *   that is not equal to match.
   */

  bool isNotEnd ();
  /* Is iterator before the end?
   * Return: False if iterator is at the grammer's end, true otherwise.
   */

  MatchLeftIterator & operator++ ();
  // Pre-Increment Operator: Skips rules with a non-matching lhs.
  MatchLeftIterator operator++ (int);
  // Post-Increment Operator: Skips rules with a non-matching lhs.

  Rule & operator* ();
  // Derefernce Operator
  Rule * operator-> ();
  // Member Dereference Operator
};

#endif//CFGRAMMER_HPP
