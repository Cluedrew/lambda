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
    StateT from;
    TransT by;
    StateT to;
  };

  std::set<StateT_> states;
  std::map<StateT_, PayloadT_> data;
  std::vector<Edge> transitions;
  StateT start;

  bool isState (StateT_ state) const
  { return states.count(state); }
};

#define STATES_HPP
