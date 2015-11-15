#include "cfg-parser.hpp"

// Implementation of the CFGParser.

#include "parse-node.hpp"
#include "token.hpp"

// Helper Classes ============================================================
// Just little bundles of short-cuts.

// The data type that repersents the stack in cfg parser.
template<typename StateT_, typename TransT_>
struct CFGStack
{
  // The stack alternates between TransT_ and StateT_.
  std::stack<std::pair<TransT_, StateT_> stack;

  // Check to see if the stack is empty.
  // Later functions only work when this is not true.
  bool isEmpty ()
  { return stack.empty(); }

  // Push a new TransT_ StateT_ pair onto the stack.
  void push (TransT_ by, StateT_ to)
  { stack.push(make_pair<TransT_, StateT_>(by, to); }

  // Cull the top item from the stack.
  void cull ()
  { stack.pop(); }

  // Peek at the last Transition used.
  TransT_ peekTrans () { return stack.top().first; }
  // Peek at the last/currant State.
  StateT_ peekState () { return stack.top().second; }
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
  // stack
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
  { return stack.top().getHead(); }

  // Push a new node onto the stack.
  void pushNode (ParseNode node)
  { stack.push(node); }

  // Pop the node from the stack.
  ParseNode popNode ()
  {
    ParseNode node(stack.top());
    stack.pop();
    return node;
  }
}

// ===========================================================================
// Constructors and Deconstructor ============================================
CFGParser::CFGParser (ActionTable) : {}
CFGParser::CFGParser (StateMachine) : {}
CFGParser::CFGParser (std::vector<Rules>) : {}

CFGParser::~CFGParser () {}

// Implementation Functions ==================================================
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
  // Set-Up ------------------------------------------------------------------
  // The stack of states and transitions.
  CFGStack<int,Token> stack;

  // The lookAhead stack, unproccessed symbols from the stream.
  LookAhead lookAhead (tokens);

  // Determain the Next Action -----------------------------------------------
  while (/* Stream is not empty and eof not reached */)
  {

    // Preform the Currant Action --------------------------------------------
    // Shift Action
    if ()
    {}
    // Reduce Action
    else
    {}
    // Done Action?
  }

  // Clean-Up ----------------------------------------------------------------
  // Any final checks, free extra memory and return.
}
