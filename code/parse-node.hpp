#ifndef PARSE_NODE_HPP
#define PARSE_NODE_HPP

#include "parse-fwd.hpp"
class Token;
class Rule;

// A node in the parse tree, an anonotated symbol.
class ParseNode
{
private:
  SymbolT head;
  std::vector<ParseNode /* * */> children;
  TextT text;

protected:
public:
  ParseNode (Token const &);
  /* Construct a terminal ParseNode from a Token.
   */

  ParseNode (SymbolT, std::vector<ParseNode /* * */> &);
  ParseNode (SymbolT, std::vector<ParseNode /* * */> &, Rule const &);
  /* Construct a nonterminal ParseNode from a SymbolT and vector of child
   *   nodes. Optional Rule may be provided as a check.
   */

  bool isTerminal () const;
  bool isNonTerminal () const;
  /* Check to see if the node is a terminal or non-terminal node.
   */

  unsigned int size () const;
  /* Get the number of children of this node.
   * A terminal node will always have 0 children. A nonterminal node can have
   * zero or more children.
   */

  ParseNode & child (unsigned int i);
  ParseNode const & child (unsigned int i) const;
  /* Get a reference to the ith child of this node.
   * Params:
   * Return:
   * Except:
   */

  Rule getRule () const; // Rule is recalcluated.
  SymbolT getHead () const; // lhs
  SymbolT getNSymbol (unsigned int) const; // rhs[n] Get head of nth child.
  TextT getText () const; // Get the text.
};

#endif//PARSE_NODE_HPP
