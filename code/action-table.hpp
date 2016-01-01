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
#include <iosfwd>
#include "parse-fwd.hpp"
#include "sr-op.hpp"
class ActionTableGenerator;

// The basic action table contains only the data used in the final calculation
class ActionTable
{
private:
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
   * Return: A copy of the SROp, or the default error operation if the SROp
   *   does not exist.
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

  size_t numOfOps () const;
  /* Get the number of operations in the ActionTable.
   * Return: The number of ops.
   */

  friend std::ostream & operator<< (std::ostream &, ActionTable const &);
  friend std::istream & operator>> (std::istream &, ActionTable &);
};

std::ostream & operator<< (std::ostream &, ActionTable const &);
/* Print an ActionTable to a stream.
 * Params: The output stream to print to and the ActionTable to print.
 * Effect: The ActionTable is printed to the ostream.
 * Return: A reference to the provided ostream.
 * Format: N(
 *   STATE SYMBOL OPERATION){N}\n
 * The number of operations in the table followed by all the operations,
 * with a trailing new line.
 */

std::istream & operator>> (std::istream &, ActionTable &);
/* Read an ActionTable from a stream.
 * Params: The input stream to read from and a reference to the ActionTable
 *   variable to store the data in.
 * Effect: The ActionTable is changed to store the value read from the ostream
 *   and the stream's badbit is set if there is an error.
 * Return: A reference to the provided istream.
 * Format: N(
 *   STATE SYMBOL OPERATION){N}\n
 * The number of operations in the table followed by all the operations,
 * with a trailing new line.
 */

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
