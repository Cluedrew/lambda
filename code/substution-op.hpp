#ifndef SUBSTUTION_OP_HPP
#define SUBSTUTION_OP_HPP

// Structure that repersents a substution operation.
// [ replaces / replaced ]

class Element;

struct SubstutionOp
{
  char replaced;
  Element * replaces;
};

#endif//SUBSTUTION_OP_HPP
