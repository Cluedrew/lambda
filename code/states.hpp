#ifndef STATES_HPP
#define STATES_HPP

/* Defining a generic state machine that can be used in other calculations.
 * Because it may actually be used in multiple places in the code base it is
 * a template to allow for different types to be used.
 *
 * This is similar to a dirrected graph, and uses some of the terminoligy
 * where simplified.
 *
 * States
 */

#include <set>
#include <map>
#include <vector>
#include <functional>
#include "parse-fwd.hpp" // Defines StateT

template<typename LabelT_, typename TransT_,
         typename LabelEquals = std::equal_to<LabelT_> >
class StateMachine
{
private:
  struct StateData
  {
    StateData (LabelT_);
    LabelT_ label;
    std::map<TransT_, StateT> outgoing;
  };
  std::vector<StateData> states;
  StateT start;

protected:
public:
  //StateMachine ();

  bool isState (LabelT_ const & label) const;
  bool isState (StateT  const & state) const;
  /* Check to see if a given state identifer or label is defined.
   * Params: label/state, the label or id to check.
   * Return: If the parameter reperents a state returns true,
   *   false otherwise.
   */

  StateT const & lookUp (LabelT_ const & label) const;
  LabelT_ const & lookUp (StateT const & state) const;
  /* Translate the a state into a label or vise-versa.
   * Params: The label/id of a state.
   * Return: The id/label of a state.
   * Except: invalid_argument if label/state is not a valid state.
   */

  std::pair<bool, StateT> addState (LabelT_ const & label);
  // Attempt to add a new state with the default payload.

  bool isTrans (StateT from, TransT_ by);
  // Is a given transition/edge out from a state defined?

  StateT getDest (StateT from, TransT_ by);
  // Get the destination of a movement from a state by a transition.

  StateT getStart (void);
  /* Get the starting state of the state machine.
   * Return: The state id of the starting state. If it has not been set
   *   the default starting state is 0.
   */

  void setStart (StateT);
  /* Set the starting state of the state machine.
   * Params: The state id of the new starting state.
   * Except: invalid_argument if start is set to a state id that is not a
   *   state.
   */
};

#include "states.tpp"

#endif//STATES_HPP
