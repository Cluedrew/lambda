#ifndef ACTION_TABLE_HPP
#define ACTION_TABLE_HPP

/* Or Prodicter Table for a LR1 parser.
 *
 * The ActionTable contains the minimum used to determain what the next
 * operation in parsing a CFG is, with some extras for error checking, while
 * the ActionTableGenerator contains to build a ActionTable from the
 * definition of the CFG, or produce information on why one cannot be built.
 */

#include <map>
#include "parse-fwd.hpp"
#include "sr-op.hpp"
class ActionTableGenerator;

// The basic action table contains only the data used in the final calculation
class ActionTable
{
private:
  //std::map<StateT, std::map<SymbolT,SROp> > data;
  std::map<std::pair<StateT, SymbolT>, SROp> data;

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
   * Except: Throws std::invalid_argument if the requested SROp does not
   *   exist.
   */

  void setOp (StateT, SymbolT, SROp);
  /* Set the operation for a given state and lookahead symbol.
   * Params: The state identifer, the symbol identifier and the SROp to be
   *   performed there.
   * Effect: Addes the given SROp to the ActionTable, or changes an existing
   *   SROp in the given position to match the new one.
   */

  void delOp (StateT, SymbolT);
  /* Remove an operation from the ActionTable.
   * Params: The StateT and SymbolT for an SROp.
   * Effect: Removes the given SROp from the ActionTable if it is defined.
   */
};

/* The ActionTableGenerator creates ActionTables for a given context-free
 * grammer. It is an pure virtual / abstract type.
 */
class ActionTableGenerator
{
private:
protected:
public:
  virtual ~ActionTableGenerator () {};

  virtual ActionTable generate () const =0;
  /* Create and return a new ActionTable from this generator.
   * Except: Throws execption if an ActionTable cannot be created.
   */

  virtual bool canGenerate () const =0;
  /* Check to see if the instance can currantly generate an ActionTable.
   * Return: True if a table can be generated, false otherwise.
   */
};

#endif//ACTION_TABLE_HPP
