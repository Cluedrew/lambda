#ifndef CFG_PARSER_HPP
#define CFG_PARSER_HPP

/* Left Right 1 parser for a context free grammer.
 */

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
