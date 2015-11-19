#ifndef SLR1_ATG_HPP
#define SLR1_ATG_HPP

/* A SLR1 ATG (Simple Left Right lookahead 1 - Action Table Genertator)
 * If my understanding is correct the type of LR parser is usually
 * differentated by how they deside which shift/reduce operation at any given
 * point. Those decitions are stored in the action table (or perdictor table)
 * so I have decided to cut out and put it hear.
 *
 * I make no promise that is how it "actually" works, but that is how it works
 * here, in this program.
 *
 * The ActionTableGenerator creates ActionTables for a given context-free
 * grammer. There maybe be some options to resolve problems.
 */

#include <iosfwd>
#include "action-table.hpp"
#include "cfgrammer.hpp"

class Slr1Atg : public ActionTableGenerator
{
private:
  std::map<StateT, std::map<SymbolT,std::vector<SROp> > > data;

  CFGrammer const grammer;

  struct SymbolData
  {
    SymbolData(SymbolT);
    bool nullable;
    std::set<SymbolT> first;
    std::set<SymbolT> follow;
  };
  std::map<SymbolT, SymbolData> symbols;

  bool calcNullable (Rule);
  bool calcFirst (Rule);
  bool calcFollow (Rule);
  void preformAllCalc ();

protected:
public:
  Slr1Atg (CFGrammer);
  /* Create a basic Slr1Atg for the given CFG.
   * Params: The definition of the context-free grammer.
   */

  virtual ~Slr1Atg ();

  bool isNullable (SymbolT) const;
  /* Check to see if a SymbolT can expand to nothing (an empty series).
   * Params: The SymbolT to look up.
   * Return: True if the SymbolT is nullable, false otherwise.
   */
  std::set<SymbolT> firstSet (SymbolT) const;
  /* Get the set of symbols that can be first in an expantion of a SymbolT.
   * Params: The SymbolT to get the first symbols for.
   * Return: The set of first symbols.
   */
  std::set<SymbolT> followSet (SymbolT) const;
  /* Get the set of symbols that can be follow the expanition of a SymbolT.
   * Params: The SymbolT to get the follow symbols for.
   * Return: The set of follow symbols.
   */

  ActionTable generate () const;
  /* Create and return a new ActionTable from this generator.
   * Except: Throws execption if an ActionTable cannot be created.
   */

  bool canGenerate () const;
  /* Check to see if the instance can currantly generate an ActionTable.
   * Return: True if a table can be generated, false otherwise.
   */
  void printProblems (std::ostream &) const;
  /* Print the problems that prevent a ActionTable from being generated.
   * Params: Stream to print to.
   * Effect: Characters are sent to stream.
   * Format:
   */
};

#endif//SLR1_ATG_HPP
