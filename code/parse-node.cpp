#include "parse-node.hpp"

// Implementation of the ParseNode class.

#include <stdexcept>
#include "token.hpp"
#include "rule-item.hpp"

// Construct a terminal ParseNode from a Token.
ParseNode::ParseNode (Token const & token) :
  head(token.kind), children(), text(token.text)
{}

// Construct a nonterminal ParseNode from a SymbolT and vector of child
//   nodes. Optional Rule may be provided as a check.
ParseNode::ParseNode (SymbolT head, std::vector<ParseNode /* * */> & kids) :
  head(head), children(kids), text()
{}

// Construct a nonterminal ParseNode from a SymbolT and vector of child
//   nodes. Optional Rule may be provided as a check.
ParseNode::ParseNode (SymbolT head, std::vector<ParseNode /* * */> & kids,
                      Rule const & rule) :
  head(head), children(kids), text()
{
  bool error = false;
  if (rule.lhs != head || children.size() != rule.cr())
    error = true;
  else
    for (unsigned int i = 0 ; i < rule.cr() ; ++i)
      if (rule.rhs[i] != getISymbol(i))
      {
        error = true;
        break;
      }

  if (error)
    throw std::bad_argument("ParseNode constructor check failed.");
}

// Check to see if the node is a terminal or non-terminal node.
bool ParseNode::isTerminal () const
{}

// Check to see if the node is a terminal or non-terminal node.
bool ParseNode::isNonTerminal () const
{}

// Getter Functions ==========================================================

// Get the number of children of this node.
unsigned int ParseNode::size () const
{
  return children.size();
}

// Get a reference to the ith child of this node.
ParseNode & ParseNode::child (unsigned int i)
{
  if (i >= children.size())
    throw std::out_of_range("ParseNode::child out_of_range");
  return children[i];
}
ParseNode const & ParseNode::child (unsigned int i) const
{
  if (i >= children.size())
    throw std::out_of_range("ParseNode::child out_of_range");
  return children[i];
}

// Get the Production Rule this node reperents.
Rule ParseNode::getRule () const
{
  Rule fin;
  fin.lhs = head;
  for (unsigned int i = 0 ; i < children.size() ; ++i)
    fin.rhs.emplace_back(children[i]);
  return fin;
}

// Get the "head" symbol, the token kind or the lhs of the Rule.
SymbolT ParseNode::getHead () const
{ return head; }

// Get the head of the ith child, or the ith symbol of the Rule's rhs.
SymbolT ParseNode::getISymbol (unsigned int i) const
{ return children[i].getHead(); }

// Get the text from a non-terminal node.
TextT ParseNode::getText () const
{ return text; }
