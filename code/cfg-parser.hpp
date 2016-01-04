#ifndef CFG_PARSER_HPP
#define CFG_PARSER_HPP

/* Left Right 1 parser for a context free grammer.
 * It gets its set of Shift/Reduce operations from an ActionTable to process
 * the Tokens from a TokenStream.
 */

#include <vector>
#include <iosfwd>
#include "action-table.hpp"
#include "cfgrammer.hpp"
class ParseNode;
class Token;
class Tokenizer;

class CFGParser
{
private:
  CFGrammer grammer;
  ActionTable actions;

protected:
public:
  CFGParser (CFGrammer const &, ActionTable const &);
  /* Create a parser from the ActionTable that speifies its actions.
   * Params: A constant reference to a CFGrammer and an ActionTable,
   *   which are copied.
   */

  virtual ~CFGParser ();

  ParseNode * parse (Tokenizer &);
  /* Parse a stream of tokens from a Tokenizer producing a parse tree.
   * Params: A Tokenizer to read from.
   * Effect: Reads all tokens from the stream until a done op is found.
   * Return: A pointer to the ParseNode at the root of the parse tree, caller
   *   must free.
   */

  void printInternal (std::ostream &) const;
  /* Print the CFGrammer and ActionTable within the Parser to a stream.
   * Params: An output stream to print to.
   * Effect: Prints to stream.
   * Format:
   *   PARSER: CFGrammer
   *   <The cf grammer>
   *   PARSER: ActionTable
   *   <The action table>
   *   PARSER: end
   */
};

#endif//CFG_PARSER_HPP
