#ifndef PARSE_NODE_HPP
#define PARSE_NODE_HPP

#include <vector>
#include "parse-fwd.hpp"

// A node in the parse tree, an anonotated symbol.
class ParseNode
{
private:
  SymbolT head;
  std::vector<ParseNode*> children;
  TextT text;

protected:
public:
  ParseNode (Token const &);
  /* Construct a terminal ParseNode from a Token.
   * Params: A constant reference to a Token.
   */

  ParseNode (SymbolT, std::vector<ParseNode*> &);
  ParseNode (SymbolT, std::vector<ParseNode*> &, Rule const &);
  /* Construct a nonterminal ParseNode from a SymbolT and vector of child
   *   nodes. Optional Rule may be provided as a check.
   * Params: A "head" SymbolT to reperent this node, a reference to a vector
   *   of child ParseNodes, the contents of which are moved into the ParseNode
   *   and optionally a constant reference to a Rule.
   * Effect: Empties the vector.
   * Except: Throws std::invalid_argument if the check does not match.
   *
   * Check: If the Rule is provided than the SymbolT must match the Rule's lhs
   *   and the vector must be the same length as the Rule's rhs and have the
   *   same symbols in the same order.
   */

  bool isTerminal () const;
  bool isNonTerminal () const;
  /* Check to see if the node is a terminal or non-terminal node.
   * Return: True if the node is of the given type, false otherwise.
   */

  unsigned int size () const;
  /* Get the number of children of this node.
   * A terminal node will always have 0 children. A nonterminal node can have
   * zero or more children.
   */

  ParseNode * child (unsigned int i);
  ParseNode const * child (unsigned int i) const;
  /* Get a pointer to the ith child of this node.
   * Params: An unsigned integer i less than size().
   * Return: A reference, possibly constant, to the ith child of this node.
   * Except: Throws std::out_of_range if i is out of range.
   */

  SymbolT getHead () const;
  /* Get the "head" symbol, the token kind or the lhs of the Rule.
   * Return: A copy of the symbol.
   */
  TextT getText () const;
  /* Get the text from a non-terminal node.
   * Return: A copy of the text in the Token used to make the node.
   */
  Rule getRule () const;
  /* Get the Production Rule this node reperents.
   * Return: A Rule that shows the production from this node to its children.
   * Timing: O(n) where n = size()
   */
  SymbolT getISymbol (unsigned int i) const;
  /* Get the head of the ith child, or the ith symbol of the Rule's rhs.
   * Params: An unsigned integer i less than size().
   * Return: A copy of the head of the ith child.
   */
};

#endif//PARSE_NODE_HPP
