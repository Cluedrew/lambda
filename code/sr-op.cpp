#include "sr-op.hpp"

// Implementation for SROp
// for error use: (char*)value? Little bit danguours.

#include <stdexcept>
#include <string>
#include <ios>
#include <istream>
#include <ostream>

// Constructors and Deconstructor ============================================
// Hidden Constructor
SROp::SROp (SROpType t, unsigned int v) :
  type(t), value(v)
{}

// Copy Constructor
SROp::SROp (SROp const & other) :
  type(other.type), value(other.value)
{}

// Default Constructor - creates error message.
SROp::SROp () :
  type(error), value(0)
{}

SROp::~SROp ()
{}

// These named constructors are intended for use by the user.
SROp SROp::shiftOp (StateT destination)
{ return SROp(shift, destination); }

SROp SROp::reduceOp (unsigned int ruleNumber)
{ return SROp(reduce, ruleNumber); }

SROp SROp::doneOp ()
{ return SROp(done, 0); }

SROp SROp::errorOp ()
{ return SROp(error, 0); }

// Type Check Functions ======================================================
SROp::SROpType SROp::getType () const
{ return type; }

bool SROp::isShift () const
{ return shift == type; }

bool SROp::isReduce () const
{ return reduce == type; }

bool SROp::isDone () const
{ return done == type; }

bool SROp::isError () const
{ return error == type; }

// Getters Functions =========================================================
StateT SROp::getDest () const
{
  if (shift != type)
    throw std::logic_error("SROp: getDest: operation not a shift");
  return value;
}

unsigned int SROp::getRule () const
{
  if (reduce != type)
    throw std::logic_error("SROp: getRule: operation not a reduce");
  return value;
}

// Input and Output Operators ================================================
std::string const shiftStr("shift");
std::string const reduceStr("reduce");
std::string const doneStr("done");
std::string const errorStr("error");

// Output
std::ostream & operator<< (std::ostream & out, SROp const & op)
{
  switch (op.getType())
  {
  case SROp::shift:
    out << std::string("shift ") << op.getDest();
    break;
  case SROp::reduce:
    out << std::string("reduce ") << op.getRule();
    break;
  case SROp::done:
    out << std::string("done");
    break;
  case SROp::error:
    out << std::string("error");
    break;
  }
  return out;
}

// Input
std::istream & operator>> (std::istream & in, SROp & op)
{
  std::string str;
  in >> str;
  if (str == "shift")
  {
    StateT dest;
    in >> dest;
    op = SROp::shiftOp(dest);
  }
  else if (str == "reduce")
  {
    unsigned int rule;
    in >> rule;
    op = SROp::reduceOp(rule);
  }
  else if (str == "done")
  {
    op = SROp::doneOp();
  }
  else if (str == "error")
  {
    op = SROp::errorOp();
  }
  else
  {
    // I hope bad is the right flag, not fail.
    in.setstate (std::ios_base::badbit);
  }
  return in;
}
