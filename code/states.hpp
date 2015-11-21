#ifndef STATES_HPP
#define STATES_HPP

/* Defining a generic state machine that can be used in other calculations.
 * Because it may actually be used in multiple places in the code base it is
 * a template to allow for different types to be used.
 *
 * This is similar to a dirrected graph, and uses some of the terminoligy
 * where simplified.
 */

#include <set>
#include <map>
#include <vector>

template<typename StateT_, typename TransT_, typename PayLoadT_>
struct StateMachine
{
  struct Edge
  {
    StateT_ from;
    TransT_ by;
    StateT_ to;
  };

  std::set<StateT_> states;
  std::map<StateT_, PayloadT_> data;
  std::vector<Edge> transitions;
  StateT start;

  bool isState (StateT_ state) const;
  // Check to see if a given state identifer is defined.

  bool addState (StateT_ state);
  // Attempt to add a new state with the default payload, return success.

  StateT_ getDest (StateT_ from, TransT_ by)
  // Get the destination of a movement from a state by a transition.
};

#endif//STATES_HPP
