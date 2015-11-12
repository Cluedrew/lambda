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

class CFGParser
{
private:
protected:
public:
  CFGParser (ActionTable);
  CFGParser (StateMachine);
  CFGParser (std::vector<Rules>);
  // I am unsure of which constructors will make the cut.

  virtual ~GFGParser ();

  ParseNode * parse (std::vector<Token> const &);
  ParseNode * parse (TokenStream const &);
  /* Parse a series or stream of tokens, producing a parse tree.
   * Params:
   * Effect: void?
   * Return: A pointer to the ParseNode at the root of the parse tree, caller
   *   must free.
   */
};

#endif//CFG_PARSER_HPP
