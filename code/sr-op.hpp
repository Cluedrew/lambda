#ifndef SR_OP_HPP
#define SR_OP_HPP

/* A structure definiting a shift/reduce operation.
 *   There are actually four kinds of shift/reduce operations, one (error)
 * is implicaty stored by the absince of a op. Hence no repersentation for it
 * exists in the class.
 *   Done operations are a signal that we should be done parsing.
 *   Shift operations add the lookahead symbol to the stack and change the
 * state to the destination.
 *   Reduce operations removed a sequence of symbols and apply a reduction
 * rule to them, creating a single lookahead symbol.
 *
 *   Note that they don't actually apply these operations, simply mark that
 * they should take place. Similarly they don't state when the operation
 * should occur, they have to be looked up at the approprate time.
 *
 *   There are several constructors, they shouldn't be called, use the named
 * constructors (static functions that return SROps) which should be used
 * instead.
 */

#include <iosfwd>
#include <string>
#include "parse-fwd.hpp"

struct SROp
{
  enum SROpType
  {
    shift,
    reduce,
    done,
    error
  };

  static SROp shiftOp (StateT destination);
  /* Create a new shift SROp and return it.
   * Params: The state that the parser should be in after shifting.
   * Return: A new SROp object that isShift.
   */

  static SROp reduceOp (unsigned int ruleNumber);
  /* Create a new reduce SROp and return it.
   * Params: The number (position) of a rule within the
   * Return: A new SROp object that isReduce.
   */

  static SROp doneOp ();
  /* Create a new done SROp and return it.
   * Return: A new SROp object that isDone.
   */

  static SROp errorOp ();
  /* Create a new error SROp and return it.
   * Return: A new SROp object that isError.
   */

  SROpType getType() const;
  /* Get the type of SROp.
   * Return: The SROpType of this SROp.
   */

  bool isShift () const;
  bool isReduce () const;
  bool isDone () const;
  bool isError () const;
  /* Check to see if it is a shift or a reduce operation.
   * Return: True if the operation is of the given type, false if it is
   *   of a different other type.
   */

  StateT getDest () const;
  /* Get the destination state of this shift operation.
   * Return: The state that should be transphered to as part of the shift.
   * Except: Throws std::logic_error if this is not a shift operation.
   */

  unsigned int getRule () const;
  /* Get the rule used in the reduction.
   * Return: The rule number that is used in this reduction.
   * Except: Throws std::logic_error if this is not a reduce operation.
   */

  std::string getMsg () const;
  /* Get the error message from the error operation.
   * Return: A copy of the error message as string object.
   * Except: Throws std::logic_error if this is not an error operation.
   */

  SROp (); // Default constructor creates an error op.
  SROp (SROp const &);
  ~SROp ();
private:
  SROpType type;
  unsigned int value;
  SROp (SROpType, unsigned int);
};

std::ostream & operator<< (std::ostream &, SROp const &);
/* Print an SROp to a stream.
 * Params: A reference to the output stream to print to and a constant
 *   reference to the SROp to print.
 * Effect: Stream is printed to.
 * Return: A reference to the output stream.
 * Format: (shift DEST|reduce RULE|done|error)
 *   DEST is the result of getDest, RULE is the result of getRule.
 */
std::istream & operator>> (std::istream &, SROp &);
/* Read an SROp from a stream.
 * Params: A reference to the output stream to read from and a constant
 *   reference to the SROp to store the value in.
 * Effect: Stream is read from, the SROp is modified, badbit of the stream
 *   is set if an error occurs.
 * Return: A reference to the output stream.
 * Format: (shift DEST|reduce RULE|done|error)
 *   DEST is the result of getDest, RULE is the result of getRule.
 */

#endif//SR_OP_HPP
