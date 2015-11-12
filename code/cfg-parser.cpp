#include "cfg-parser.hpp"

// Implementation of the CFGParser.

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

  // Peek at the last Transition used.
  TransT_ peekTrans () { return stack.top().first; }
  // Peek at the last/currant State.
  StateT_ peekState () { return stack.top().second; }
};

// Constructors and Deconstructor ============================================
CFGParser::CFGParser (ActionTable)
CFGParser::CFGParser (StateMachine)
CFGParser::CFGParser (std::vector<Rules>)

CFGParser::~CFGParser ()

// Parse a series or stream of tokens, producing a parse tree.
ParseNode * CFGParser::parse (std::vector<Token> const &)
{
  // This one just creates a TokenStream and passes it to its partner
}

// Parse a series or stream of tokens, producing a parse tree.
ParseNode * CFGParser::parse (TokenStream const & tokin)
{
  // Set-Up ------------------------------------------------------------------

  // Determain the Next Action -----------------------------------------------
  while (/* Stream is not empty and eof not reached */)
  {

  // Preform the Currant Action ----------------------------------------------
    // Shift Action
    if ()
    {}
    // Reduce Action
    else
    {}
  }

  // Clean-Up ----------------------------------------------------------------
  // Any final checks, free extra memory and return.
}
