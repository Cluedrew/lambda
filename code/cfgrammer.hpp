#ifndef CFGRAMMER_HPP
#define CFGRAMMER_HPP

/* A context free grammer (CFG for short) which describes a context free
 * language (CFL).
 */

#include <set>
#include <vector>
#include "parse-fwd.hpp"
#include "rule-item.hpp"

struct CFGrammer
{
  std::set<SymbolT> nonTerminals;
  std::set<SymbolT> terminals;
  std::set<Rule> rules;
  SymbolT start;
};

CFGrammer defineCFGrammer (std::istream & in);
/* Create a CFGrammer from a text definition.
 * Params: A character stream that contains the language definition.
 * Effect: The stream is read from to the end of the CFG definition.
 * Return: A new CFGrammer.
 * Except: ??
 * Format: A grammer must be speified in the following way:
 *   ???
 */

#endif//CFGRAMMER_HPP
