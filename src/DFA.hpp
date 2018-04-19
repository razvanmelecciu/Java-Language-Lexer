#ifndef _DFA_h
#define _DFA_h

#include "common.h"
#include "alphabet.hpp"

#include <string>
#include <iostream>
#include <type_traits>
#include <set>
#include <map>

AUTOMATON_START
DFA_START

/////////////////////////////////////////////////////////////////////////
/// A simple class which models a deterministic finite state automaton [Q, V, d, qo, F]
/// where Q - list of states, V - alphabet used, d - delta transition function d: Q x V -> Q,
///       q0 - initial state, F - list of final states
/// Template params: char_type (char, w_char etc.)
/// The states type: unsigned int, short etc.
/////////////////////////////////////////////////////////////////////////
template <class char_type = char,
          class states = unsigned int>
class DFA
{
  // - Public types

public:

  enum class Messages : unsigned char { RECOGNIZED, UNRECOGNIZED, INVALID_ALPHABET,
                                        NULL_SEQUENCE_SIZE, NO_STATE_FOR_CHAR,
                                        NO_STATES, NO_FINAL_STATES, INVALID_STATE };  ///< Errors
  typedef states states_type;                                                         ///< The state type
  typedef char_type alphabet_char_type;                                               ///< The alphabet char type

private :

  // - Internal types

  typedef std::pair<states_type, alphabet_char_type> delta_state_transition;              ///< The transition (crt state, character)
  typedef std::map<delta_state_transition, states_type> delta_transition_states_mapping;  ///< The actual list of transitions and the delta function

public :

  /// Default ctor
  DFA() : initial_state_q0_(0)
  {
  }

  /// Copy ctor
  DFA(const DFA& RHS_Object) : final_states_set_f_(RHS_Object.final_states_set_f_), 
                               initial_state_q0_(RHS_Object.initial_state_q0_),
                               delta_states_mapping_qd_(RHS_Object.delta_states_mapping_qd_)
  {
  }

#if defined MOVE_SEMANTICS
  /// Move ctor
  DFA(DFA&& RHS_Orphaned) : final_states_set_f_(static_cast<std::set<states_type>&&>(RHS_Object.final_states_set_f_)),
                            initial_state_q0_(RHS_Object.initial_state_q0_),
                            delta_states_mapping_qd_(static_cast<delta_transition_states_mapping&&>(RHS_Object.delta_states_mapping_qd_))
  {
  }
#endif

  /// Dtor
  ~DFA()
  {
  }

  // - Operators

  /// Copy assignment
  DFA& operator = (const DFA& RHS_Object)
  {
    if (this != &RHS_Object)
    {
      final_states_set_f_ = RHS_Object.final_states_set_f_;
      initial_state_q0_ = RHS_Object.initial_state_q0_;
      delta_states_mapping_qd_ = RHS_Object.delta_states_mapping_qd_;
    }

    return *this;
  }

#if defined MOVE_SEMANTICS
  /// Move assignment
  DFA& operator = (DFA&& RHS_Orphaned)
  {
    if (this != &RHS_Object)
    {
      final_states_set_f_ = RHS_Orphaned.final_states_set_f_;
      initial_state_q0_ = RHS_Orphaned.initial_state_q0_;
      delta_states_mapping_qd_ = RHS_Orphaned.delta_states_mapping_qd_;
    }

    return *this;
  }
#endif

  // - Accessor methods

  /// Get the number of final states
  std::size_t NoFinalStates() const
  {
    return final_states_set_f_.size();
  }

  /// Check if the language denoted by this DFA recognizes the string specified (std::string val.c_str() or std::wstring val.c_str())
  Messages Accepts(alphabet_char_type* crt_sequence, std::size_t sequence_size) const
  {
    if (sequence_size == 0)
      return Messages::NULL_SEQUENCE_SIZE;

    delta_state_transition crt_transition;
    delta_transition_states_mapping::const_iterator my_crt_position(delta_states_mapping_qd_.begin());
    delta_transition_states_mapping::const_iterator states_mapping_end(delta_states_mapping_qd_.end());
    states_type new_state(initial_state_q0_);

    crt_transition.first = initial_state_q0_;
    crt_transition.second = crt_sequence[0];                              // the first stage is q0, first_char

    std::size_t i = 0;
    while (i < sequence_size)
    {
      ++i;

      my_crt_position = delta_states_mapping_qd_.find(crt_transition);    // find the current state from the crt_transition
      if (my_crt_position != states_mapping_end)
      {
        new_state = my_crt_position->second;                              // this is the new state -> switch to the new state
        crt_transition.first  = new_state;
        crt_transition.second = *(crt_sequence + i);                      // go to the next input from the sequence
      }
    }

    std::set<states_type>::const_iterator final_state;
    final_state = final_states_set_f_.find(new_state);                    // check if my last state can be found in my set of final states

    if (final_state != final_states_set_f_.end())
      return Messages::RECOGNIZED;
    else
      return Messages::UNRECOGNIZED;
  }

  // - Mutating methods

  /// Set the value of the initial state q0
  void SetInitialState(states_type my_initial_state)
  {
    initial_state_q0_ = my_initial_state;
  }

  /// Adds a final state
  bool AddFinalState(states_type my_final_state)
  {
    std::set<states_type>::_Pairib ret = final_states_set_f_.insert(my_final_state);
    return ret.second;
  }

  /// Remove a final state
  bool RemoveFinalState(states_type my_final_state)
  {
    auto my_iterator = final_states_set_f_.find(my_final_state);
    if (my_iterator != final_states_set_f_.end())
    {
      final_states_set_f_.erase(my_iterator);
      return true;
    }

    return false;
  }

  /// Clear all (states + mapping)
  void ClearAll()
  {
    final_states_set_f_.clear();
    delta_states_mapping_qd_.clear();
  }

  /// Adds a transition along with a state
  bool AddTransition(states_type old_state, alphabet_char_type character, states_type new_state)
  {
    delta_state_transition crt_transition(old_state, character);
    delta_transition_states_mapping::_Pairib ret = delta_states_mapping_qd_.insert(delta_transition_states_mapping::value_type(crt_transition, new_state));
    return ret.second;
  }

private :

  // - Members

  std::set<states_type>            final_states_set_f_;        ///< the final states
  states_type                      initial_state_q0_;          ///< the initial state
  delta_transition_states_mapping  delta_states_mapping_qd_;   ///< a simple map that keeps the states and the delta function
};

DFA_END
AUTOMATON_END

#endif // _DFA_h

