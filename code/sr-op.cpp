#include "sr-op.hpp"

// Constructors
SROp SROp::shiftOp (StateT destination)
...

SROp SROp::reduceOp (...)
...

SROp SROp::doneOp ()
...

// Predicate Functions
bool SROp::isShift ()
{ return shift == type; }

bool SROp::isReduce ()
{ return reduce == type; }

bool SROp::isDone ()
{ return done == type; }

// Getters
StateT getDest ()
{
  if (shift != type)
    throw ...;
  return to;
}

_reduce_type_ getRule ()
{
  if (reduce != type)
    throw ...;
  return by;
}
