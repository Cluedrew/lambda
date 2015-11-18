#ifndef ACTION_TABLE_HPP
#define ACTION_TABLE_HPP

/* Or Prodicter Table for a LR1 parser.
 *
 * The ActionTable contains the minimum used to determain what the next
 * operation in parsing a CFG is, with some extras for error checking, while
 * the ActionTableGenerator contains to build a ActionTable from the
 * definition of the CFG, or produce information on why one cannot be built.
 */

#include <iosfwd>
#include <map>
#include "parse-fwd.hpp"
#include "cfgrammer.hpp"
#include "sr-op.hpp"
class ActionTableGenerator;

typedef int StateT;

// The basic action table contains only the data used in the final calculation
class ActionTable
{
private:
  std::map<StateT, std::map<SymbolT,SROp> > data;

protected:
public:
  ActionTable ();
  // Create an unfilled ActionTable.

  ActionTable (ActionTableGenerator const &);
  /* Create and fill an ActionTable from a generator.
   * Params: A constant reference to an ActionTableGenerator that can generate
   *   an ActionTable.
   * Except:
   */

  virtual ~ActionTable ();

  ActionTable & operator= (ActionTableGenerator const &);
  /* Replace all SROps in the ActionTable with those from a generator.
   * Params: A constant reference to an ActionTableGenerator that can generate
   *   an ActionTable.
   * Except:
   */

  bool hasOp (StateT, SymbolT) const;
  /* Check to see if a given state and transition symbol has a defined SROp.
   * Params: The StateT and SymbolT lookup for a possible SROp.
   * Return: True is a SROp is defined for that StateT and SymbolT.
   */

  SROp getOp (StateT, SymbolT) const;
  /* Get the SROp for a given state
   * Params: The StateT and SymbolT lookup for an SROp.
   * Return: A copy of the SROp.
   * Except:
   */

  void setOp (StateT, SymbolT, SROp);
  /* Set the operation for a given state and lookahead symbol.
   * Params: The state identifer, the symbol identifier and the SROp to be
   *   performed there.
   * Effect: Addes the given SROp to the ActionTable.
   * Except:
   */

  void delOp (StateT, SymbolT);
  /* Remove an operation from the ActionTable.
   * Params: The StateT and SymbolT for an SROp.
   * Effect: Removes the given SROp from the ActionTable.
   * Except:
   */
};

/* The ActionTableGenerator creates ActionTables for a given context-free
 * grammer. There maybe be some options to resolve problems.
 */
class ActionTableGenerator
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
  ActionTableGenerator (CFGrammer);
  /* Create a basic ActionTableGenerator for the given CFG.
   * Params: The definition of the context-free grammer.
   */

  virtual ~ActionTableGenerator ();

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

#endif//ACTION_TABLE_HPP
