#ifndef SR_OP_HPP
#define SR_OP_HPP

/* A structure definiting a shift or reduce operation.
 *
 * They are stored with regards to their origin state and the value of the
 * look ahead so that information is not stored here again.
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

  StateT to;

  bool isShift ();
  bool isReduce ();
  /* Check to see if it is a shift or a reduce operation.
   * Return: True if the operation is of the given type, false if it is
   *   of the other type.
   */

  int reduceSize ();
  /* The number of the proceding simples reduced into one.
   */

  SROp doneOp ();
  { return SROp{done, 0}; }
  // Get a new done SROp.
};

#endif//SR_OP_HPP
