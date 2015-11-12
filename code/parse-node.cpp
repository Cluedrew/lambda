#include "parse-node.hpp"

// Implementation of the ParseNode class.

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
  {}
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
{ return children[i]; }
ParseNode const & ParseNode::child (unsigned int i) const
{ return children[i]; }

// Rule is recalcluated.
Rule ParseNode::getRule () const
{
  Rule fin;
  fin.lhs = head;
  for (unsigned int i = 0 ; i < children.size() ; ++i)
    fin.rhs.emplace_back(children[i]);
  return fin;
}

// lhs
SymbolT ParseNode::getHead () const
{ return head; }

// rhs[n] Get head of ith child.
SymbolT ParseNode::getISymbol (unsigned int i) const
{ return children[i].getHead(); }

// Get the text.
TextT ParseNode::getText () const
{ return text; }
