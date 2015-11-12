#ifndef STATES_HPP
#define STATES_HPP

/* Defining a generic state machine that can be used in other calculations.
 *
 * This is similar to a dirrected graph.
 */

typedef ? state_t;
typedef ? trans_t;
typedef ? payload_t;

#include <set>
#include <vector>

// Will this have to be a template?
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
