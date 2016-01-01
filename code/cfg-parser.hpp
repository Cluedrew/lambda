#ifndef CFG_PARSER_HPP
#define CFG_PARSER_HPP

/* Left Right 1 parser for a context free grammer.
 * It gets its set of Shift/Reduce operations from an ActionTable to process
 * the Tokens from a TokenStream.
 */

#include <vector>
#include "action-table.hpp"
class ParseNode;
class Token;

class CFGParser
{
private:
  ActionTable actions;

protected:
public:
  CFGParser (ActionTable const &);
  /* Create a parser from the ActionTable that speifies its actions.
   * Params: A constant reference to an ActionTable, which is copied.
   */

  virtual ~GFGParser ();

  ParseNode * parse (std::vector<Token> const &);
  ParseNode * parse (TokenStream const &);
  /* Parse a series or stream of tokens, producing a parse tree.
   * Params: A TokenStream to read from.
   * Effect: (stream only) Reads all tokens from the stream.
   * Return: A pointer to the ParseNode at the root of the parse tree, caller
   *   must free.
   */
};

#endif//CFG_PARSER_HPP
