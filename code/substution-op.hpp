#ifndef SUBSTUTION_OP_HPP
#define SUBSTUTION_OP_HPP

// Structure that repersents a substution operation.
// [ replaces / replaced ]

#include "parse-fwd.hpp"
class Element;

struct SubstutionOp
{
  TextT replaced;
  Element const & replaces;

  SubstutionOp (TextT var, Element const & val) :
    replaced(var), replaces(val)
  {}
};

#endif//SUBSTUTION_OP_HPP
