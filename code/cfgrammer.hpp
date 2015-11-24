#ifndef CFGRAMMER_HPP
#define CFGRAMMER_HPP

/* A context free grammer (CFG for short) which describes a context free
 * language (CFL).
 */

#include <set>
#include <vector>
#include <isofwd>
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

#endif//CFGRAMMER_HPP
