#ifndef CFG_PARSER_HPP
#define CFG_PARSER_HPP

/* Parses a context free grammer.
 */

// A structure that repersents a cfg. (Does it get its own file?)
struct CFGrammer
{
  std::set<symbol_t> nonTerminals;
  std::set<symbol_t> termianals;
  std::vector<Rule> rules;
  symbol_t start;
};

// The data type that repersents the stack in cfg parser.
// (Move to the cpp file, is an implementation detal.)
struct CFGStack
{
  state_t startState;
  std::stack<std::pair<trans_t, state_t> stack;
};

class CFGParser
{
private:
protected:
public:
  GFGParser (StateMachine);
  GFGParser (std::vector<Rules>);

  virtual ~GFGParser ();

  ParseNode * parse (std::vector<Token> const &);
  ParseNode * parse (Tokenizer const &);
  /* Parse a series or stream of tokens, producing a parse tree.
   * Params:
   * Effect: void?
   * Return: A pointer to the ParseNode at the root of the parse tree, caller
   *   must free.
   */
};

#endif//CFG_PARSER_HPP
