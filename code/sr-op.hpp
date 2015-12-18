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
 */

struct SROp
{
  enum SROpType
  {
    shift,
    reduce,
    done//,
    //error
  } type;

  union
  {
    StateT shiftTo;
    // Is this to 'brittle'? It will break if the rules are ever moved.
    Rule * reduceBy;
  };

  bool isShift ();
  bool isReduce ();
  bool isDone ();
  /* Check to see if it is a shift or a reduce operation.
   * Return: True if the operation is of the given type, false if it is
   *   of a different other type.
   */

  SROp shiftOp (StateT destination);
  // Create a new shift SROp and return it.

  SROp reduceOp (Rule * proRule);
  // Create a new reduce SROp and return it.

  static SROp doneOp ()
  { return SROp{done, 0}; }
  // Get a new done SROp.
};

#endif//SR_OP_HPP
