#ifndef SUBSTUTION_OP_HPP
#define SUBSTUTION_OP_HPP

// Structure that repersents a substution operation.
// [ replaces / replaced ]

#include "parse-fwd.hpp"
class LambdaElement;

struct SubstutionOp
{
  TextT replaced;
  LambdaElement const & replaces;

  SubstutionOp (TextT var, LambdaElement const & val) :
    replaced(var), replaces(val)
  {}
};

#endif//SUBSTUTION_OP_HPP
