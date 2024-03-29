#ifndef _DFA_h
#define _DFA_h

#include "common.h"
#include "alphabet.hpp"

#include <string>
#include <iostream>
#include <type_traits>
#include <set>
#include <map>

LEXER_START

/////////////////////////////////////////////////////////////////////////
/// A simple class which models a deterministic finite state automaton [Q, V, d, qo, F]
/// where Q - list of states, V - alphabet used, d - delta transition function d: Q x V -> Q,
///       q0 - initial state, F - list of final states
/// Template params: char_type (char, w_char etc.)
///                  states: unsigned int, short etc.
///                  state_label: an additional label associated with the final states
/////////////////////////////////////////////////////////////////////////
template <class char_type = char,
          class states = unsigned int,
          class state_label = unsigned short>
class DFA
{
  static_assert(tools::is_character<char_type>::value, "The class template can only be instantiated with char types like char/wchar_t");

  // - Public types

public:

  enum class Messages : unsigned char { RECOGNIZED, UNRECOGNIZED, NULL_SEQUENCE_SIZE, NO_FINAL_STATES}; ///< Errors
  typedef states      states_type;                                                                      ///< The state type
  typedef state_label states_label;                                                                     ///< The attached state label for each node
  typedef char_type alphabet_char_type;                                                                 ///< The alphabet char type
  typedef typename tools::if_is_of_type<char_type, wchar_t,
                                        std::wstring, std::string>::ret_type string_type;               ///< compile time type identification

protected :

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
  DFA(DFA&& RHS_Orphaned) : final_states_set_f_(static_cast<std::map<states_type, state_label>&&>(RHS_Object.final_states_set_f_)),
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
  Messages Accepts(const alphabet_char_type* crt_sequence, std::size_t sequence_size) const
  {
    if (sequence_size == 0)
      return Messages::NULL_SEQUENCE_SIZE;

    if (NoFinalStates() == 0)
      return Messages::NO_FINAL_STATES;

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

    std::map<states_type, states_label>::const_iterator final_state;
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
  bool AddFinalState(states_type my_final_state, states_label node_label)
  {
    std::map<states_type, state_label>::_Pairib ret = final_states_set_f_.insert(std::map<states_type, state_label>::value_type(my_final_state, node_label));
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

  /// Adds several transitions from the specified string
  void AddTransition(states_type old_state, const alphabet_char_type* crt_sequence, std::size_t sequence_size, states_type new_state)
  {
    delta_state_transition crt_transition;
    crt_transition.first = old_state;
    for (std::size_t i = 0; i < sequence_size; ++i)
    {
      crt_transition.second = crt_sequence[i];
      delta_states_mapping_qd_.insert(delta_transition_states_mapping::value_type(crt_transition, new_state));
    }
  }

  /// Adds several transitions from the specified string, excluding the characters mentioned
  void AddTransition(states_type old_state, const alphabet_char_type* crt_sequence, std::size_t sequence_size, 
                     const alphabet_char_type* exceptions, std::size_t exceptions_sequence_size, states_type new_state)
  {
    bool add_character = true;
    delta_state_transition crt_transition;
    crt_transition.first = old_state;

    for (std::size_t i = 0; i < sequence_size; ++i)
    {
      add_character = true;
      for (std::size_t j = 0; j < exceptions_sequence_size; ++j)
      {
        if (crt_sequence[i] == exceptions[j])
        {
          add_character = false;
          break;
        }
      }

      if (add_character)                                    // insert characters that are not found in the exceptions list
      {
        crt_transition.second = crt_sequence[i];
        delta_states_mapping_qd_.insert(delta_transition_states_mapping::value_type(crt_transition, new_state));
      }
    }
  }

protected :

  // - Members

  std::map<states_type, state_label> final_states_set_f_;        ///< the final states
  states_type                        initial_state_q0_;          ///< the initial state
  delta_transition_states_mapping    delta_states_mapping_qd_;   ///< a simple map that keeps the states and the delta function
};

LEXER_END

#endif // _DFA_h

