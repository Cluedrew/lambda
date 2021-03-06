#ifndef STATES_EPP
#define STATES_EPP

#include <stdexcept>

//Trying to cut down on the much to big template headers.
#define TEMPLATE_HEAD(return_t) \
template<typename LabelT_, typename TransT_, typename LabelEquals> \
return_t StateMachine<LabelT_, TransT_, LabelEquals>

// I just need this to keep the comma from bein miss read.
typedef std::pair<bool, StateT> pairBoolState;

// State Helper Structure Implementation =====================================
// Create a new state.
TEMPLATE_HEAD()::StateData::StateData (LabelT_ label) :
    label(label), outgoing()
{}

// StateMachine Functions ====================================================

// Check to see if a given state identifer or label is defined.
TEMPLATE_HEAD(bool)::isState (LabelT_ const & label) const
{
  // The labels have to all be checked with LabelEquals.
  for (unsigned int i = 0 ; i < states.size() ; ++i)
    if (cmp(label, states[i].label))
      return true;
  return false;
}
TEMPLATE_HEAD(bool)::isState (StateT const & state) const
{
  // The state ids are continuous, they just have to be in range.
  return (0 <= state && state < states.size());
}

// Translate the a state into a label or vise-versa.
TEMPLATE_HEAD(StateT)::lookUp (LabelT_ const & label) const
{
  for (StateT i = 0 ; i < states.size() ; ++i)
    if (cmp(label, states[i].label))
      return i;
  // Error on fall through.
  throw std::invalid_argument("StateMachine: lookUp: invalid label value.");
}
TEMPLATE_HEAD(LabelT_ const &)::lookUp (StateT state) const
{
  // Check that state is a State.
  if (!isState(state))
    throw std::invalid_argument("StateMachine: lookUp: invalid state value.");

  return states[state].label;
}

// Attempt to add a new state with the default payload, return success.
TEMPLATE_HEAD(pairBoolState)::addState (LabelT_ const & label)
{
  // If the state label is already defined, return the approprate id.
  if (isState(label))
    return std::make_pair(false, lookUp(label));
  // If it is new, add a new state and return the new id.
  else
  {
    states.emplace_back(label);
    return std::make_pair(true, states.size() - 1);
  }
}

// Is a given transition/edge out from a state defined?
TEMPLATE_HEAD(bool)::isTrans (StateT from, TransT_ by)
{
  if(!isState(from))
    throw std::invalid_argument("StateMachine: isTrans: invalid from value.");

  return 1 == states[from].outgoing.count(by);
}

// Get the destination of a movement from a state by a transition.
TEMPLATE_HEAD(StateT)::getDest (StateT from, TransT_ by)
{
  if (!isState(from))
    throw std::invalid_argument("StateMachine: getDest: invalid from value.");
  else if (!isTrans(from, by))
    throw std::invalid_argument("StateMachine: getDest: invalid by value.");

  return states[from].outgoing[by];
}
// Set the trans outgoing _from_ labeled _by_ to _to_.
TEMPLATE_HEAD(void)::setTrans (StateT from, TransT_ by, StateT to)
{
  if (!isState(from))
    throw std::invalid_argument("StateMachine: setTrans: invalid from value.");
  else if (!isState(to))
    throw std::invalid_argument("StateMachine: setTrans: invalid to value.");

  states[from].outgoing[by] = to;
}

// Remove the trans outgoing _from_ labeled _by_.
TEMPLATE_HEAD(void)::delTrans (StateT from, TransT_ by)
{
  if (!isState(from))
    throw std::invalid_argument("StateMachine: delTrans: invalid from value.");

  states[from].outgoing.erase(by);
}

// Get the starting state of the state machine.
TEMPLATE_HEAD(StateT)::getStart (void)
{ return start; }

// Set the starting state of the state machine.
TEMPLATE_HEAD(void)::setStart (StateT state)
{
  if (!isState(state))
    throw std::invalid_argument("StateMachine: setStart: invalid state value.");

  start = state;
}

// Clean up local macros.
#undef TEMPLATE_HEAD

#endif//STATES_EPP
