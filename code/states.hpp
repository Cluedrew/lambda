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
#include <utlity>

// Will this have to be a template?
struct StateMachine
{
  struct Edge
  {
    state_t from;
    trans_t by;
    state_t to;
  };

  // struct StateData { state_t state; payload_t data; };

  std::set<state_t> states;
  std::vector<std::pair<state_t, payload_t> > accepting;
  //std::vector<StateData> stateData;
  std::vector<Edge> transitions;
  state_t start;

  bool isState (state_t state) const
  { return states.count(state); }
};

#define STATES_HPP
