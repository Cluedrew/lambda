#include "cfg-parser.hpp"

// Implementation of the CFGParser.

#include <iostream>
#include <utility>
#include <list>
#include "parse-node.hpp"
#include "token.hpp"



// Helper Classes ============================================================
// Just little bundles of short-cuts.

// The data type that repersents the stack in cfg parser.
template<typename StateT_, typename TransT_>
struct CFGStack
{
  // The first state, before the stack even starts.
  StateT_ startState;

  // The stack alternates between TransT_ and StateT_.
  std::stack<std::pair<TransT_, StateT_> stack;

  CFGStack(StateT_ start) :
    stateState(start), stack()
  {}

  // Check to see if the stack is empty.
  // Later functions only work when this is not true.
  bool isEmpty ()
  { return stack.empty(); }

  // Push a new TransT_ StateT_ pair onto the stack.
  void push (TransT_ by, StateT_ to)
  {
    stack.push(make_pair<TransT_, StateT_>(by, to));
  }
  void push (std::pair<TransT_, StateT_> pair)
  {
    stack.push(pair);
  }

  // Cull the top item from the stack.
  void cull ()
  { stack.pop(); }

  // Pop the top item from the stack.
  std::pair<TransT_, StateT_> pop ()
  {
    std::pair<TransT_, StateT_> tmp = stack.top();
    stack.pop();
    return tmp;
  }

  // Peek at the last Transition used.
  // Don't call when the stack is empty.
  TransT_ peekTrans ()
  {
    return stack.top().first;
  }

  // Peek at the last/currant State.
  StateT_ peekState ()
  {
    if (stack.empty())
      return startState;
    else
      return stack.top().second;
  }

  // Get the number of items on the stack (counting TransT_)
  size_t size ()
  {
    return stack.size();
  }
};



// The look ahead, a 'prefex' of the TokenStream the parser reads from.
class LookAhead
{
private:
  // This is where we store the first Tokens from the stream. Translated into
  // ParseNodes. This includes those we pushed into the stream.
  std::stack<ParseNode> stack;

  // The TokenStream the look ahead is filled from.
  TokenStream & tokStream;

  // Min-fill: If we are looking for a symbol and there isn't one in the
  // stack, add one to the stack built from the next token in the stream.
  void minFill ()
  { if (stack.empty()) stack.push(ParseNode(tokStream.next())); }

protected:
public:
  // Create a token stream
  LookAhead (TokenStream & ts) :
    stack(), tokStream(ts)
  {}

  // Look at the symbol (head of node) on the top of the stack.
  SymbolT peekSymbol ()
  {
    minFill();
    return stack.top().getHead();
  }

  // Push a new node onto the stack.
  void pushNode (ParseNode node)
  {
    stack.push(node);
  }

  // Pop the node from the stack.
  ParseNode popNode ()
  {
    minFill();
    ParseNode node(stack.top());
    stack.pop();
    return node;
  }
}



// ===========================================================================
// Constructors and Deconstructor ============================================
CFGParser::CFGParser (CFGrammer const & grammer,
                      ActionTable const & actionTable) :
  grammer(grammer), actions(actionTable)
{}

CFGParser::~CFGParser () {}



// Implementation Functions ==================================================

static ParseNode * preformReduce
    (CFGStack<StateT, ParseNode *> & stack, Rule const & rule)
/* This hidden helper prefroms a reduction, taking an Symbols off the _stack_
 * to match the rhs of _rule_ and producing the lhs of rule, made from the
 * fustion of the removed nodes.
 * Return: New node, caller must free. NULL on error.
 */
{
  // Make sure there are enough symbols.
  if (stack.size() >= rule.cr())
  {
    // Now pop, the top cr() symbols to be the children of the new nodes.
    std::list<std::pair<ParseNode *, StateT> > children;
    for (unsigned int i = 0 ; i < rule.cr() ; ++i)
      children.push_front(stack.pop());

    // Preform a check of all items.
    bool passing = true;
    for (std::list<std::pair<ParseNode *, StateT> >::const_iterator it
         = children.begin() ; it != children.end() ; ++it)
      if (rule.rhs[i] != it->second->getHead())
      {
        std::cerr << "REDUCE ERROR: Symbol mismatch, expected " <<
            rule.rhs[i] << " found " << it->second->getHead() << '.'
            << std::endl;
        passing = false;
      }

    if (passing)
    {
      // Convert the list of pairs to a vector of ptrs.
      std::vector<ParseNode*> childPtrs;
      while (!children.empty())
      {
        childPtrs.push_back(children.back());
        children.pop_back();
      }

      // Create a new ParseNode and return it.
      return new ParseNode(rule.lhs, childPtrs);
    }
    else
    {
      // To avoid memory loss push the items back on the stack.
      while (!children.empty())
      {
        stack.push(children.front());
        children.pop_front();
      }
      return NULL;
    }
  }
  // Otherwise print an error message and quit.
  else
  {
    std::cerr << "REDUCE ERROR: Not enough symbols on the stack ("
        << stack.size() << '/' << ")." << std::endl;
    return NULL;
  }
}

// Parse a series or stream of tokens, producing a parse tree.
ParseNode * CFGParser::parse (std::vector<Token> const & tokens)
{
  // This one just creates a TokenStream and passes it to its partner
  TokenStream tokStream(tokens);
  return parse (tokStream);
}

// Parse a series or stream of tokens, producing a parse tree.
ParseNode * CFGParser::parse (TokenStream const & tokens)
{
  // === Set-Up ===
  // The stack of states and transitions.
  CFGStack<StateT,ParseNode*> stack;

  // The lookAhead stack, unproccessed symbols from the stream.
  LookAhead lookAhead(tokens);

  // === Determain the Next Action ===
  bool looping = true;
  do {
    // Look-Up the current action.
    SROp curOp = actions.getOp(stack.peekState(), lookAhead.peekSymbol())

    // === Preform the Currant Action ===
    switch (curOp.getType())
    {
    // Shift Action
    case SROp::shift:
      // Move the look ahead token/symbol to the stack and change states.
      stack.push(lookAhead.popNode(), curOp.getDest())
      break;

    // Reduce Action
    case SROp::reduce:
      // Get the rule, unwind the stack, create a new symbol and put it in the
      // look ahead.
      // I think a helper function ...(stack,rule) would do nicely.
      !
      break;

    // Done Action
    case SROp::done:
      // Quite simple: we are done so stop looping.
      looping = false;
      break;

    // Error Action
    case SROp::error:
      // Print an error message and return.
      std::cerr << "PARSE ERROR: Error Operation at: (" << stack.peekState()
          << ", " << lookAhead.peekSymbol << ") reads: " << curOp.getMsg()
          << std::endl;
      return NULL;

    // default shouldn't occur.
    default:
      std::cerr << "PARSE ERROR: Bad Operation type." << std::endl;
      return NULL;
    }
  }
  // Now exit if we are done.
  while (!curOp.isDone());

  // === Clean-Up ===
  // Any final checks, free extra memory and return.

  // If there is exactly one item on the stack and the look ahead is eof ...
  if (1 == stack.size() && getEofSymbol() == lookAhead.peekSymbol())
    // ... than parsing was a success, return the parse tree.
    return stack.peekTrans();
  else
  {
    // Otherwise clean up the extra nodes.
    std::cerr << "PARSE ERROR: Bad end state. Had '" << stack.size() <<
        "' items on the stack and '" << lookAhead.peekSymbol() <<
        "' is the last symbol." << std::endl;
    while (stack.isEmpty())
    {
      delete stack.peekTrans();
      stack.cull();
    }
    return NULL;
  }
}
