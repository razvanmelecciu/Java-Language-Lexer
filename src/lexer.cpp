#include "stdafx.h"
#include "lexer.h"

LEXER_START

Lexer::Lexer(const stream_type& input_stream,
             const GrammarIdentifiers<char_type>& grammar_identifiers) : crt_stream_(input_stream),
                                                                         grammar_symbols_(new GrammarIdentifiers<char_type>), crt_position_(0),
                                                                         tokens_string_table_(new TokenStringTable<char_type>)
{
  // - Initialize the DFA with the scanning rules

  // - Initial state
  SetInitialState(0);

  // - Final states with labels
  AddFinalState(1, CHECK_MULTI_STATE);
  AddFinalState(4, MULTI_LINE_COMMENT);
  AddFinalState(5, INLINE_COMMENT);
  AddFinalState(8, CHARACTER_LITERAL);
  AddFinalState(11, STRING_LITERAL);
  AddFinalState(12, WHITE_SPACE);
  AddFinalState(13, CHECK_MULTI_STATE);
  AddFinalState(14, INTEGER_LITERAL);
  AddFinalState(15, FLOATING_POINT_LITERAL);
  AddFinalState(18, FLOATING_POINT_LITERAL);
  AddFinalState(19, FLOATING_POINT_LITERAL);
  AddFinalState(20, SEPARATOR);
  AddFinalState(21, BRACKET);
  AddFinalState(22, OPERATOR);
  AddFinalState(23, OPERATOR);
  AddFinalState(24, OPERATOR);
  AddFinalState(25, OPERATOR);
  AddFinalState(26, OPERATOR);
  AddFinalState(27, OPERATOR);
  AddFinalState(28, OPERATOR);
  AddFinalState(29, OPERATOR);
  AddFinalState(30, OPERATOR);
  AddFinalState(31, OPERATOR);
  AddFinalState(32, OPERATOR);
  AddFinalState(33, OPERATOR);
  AddFinalState(34, OPERATOR);
  AddFinalState(35, OPERATOR);
  AddFinalState(36, OPERATOR);
  AddFinalState(37, OPERATOR);
  AddFinalState(38, OPERATOR);
  AddFinalState(39, OPERATOR);
  AddFinalState(40, OPERATOR);
  AddFinalState(41, OPERATOR);
  AddFinalState(42, OPERATOR);
  AddFinalState(43, OPERATOR);
  AddFinalState(44, OPERATOR);
  AddFinalState(45, OPERATOR);
  AddFinalState(46, OPERATOR);
  AddFinalState(47, OPERATOR);
  AddFinalState(48, OPERATOR);
  AddFinalState(49, OPERATOR);
  AddFinalState(50, OPERATOR);
  AddFinalState(51, OPERATOR);
  AddFinalState(52, OPERATOR);

  // - Transition Rules for Single line comments
  AddTransition(0, '/', 1);
  AddTransition(1, '*', 2);

  for (char_type c = ' '; c <= '~'; ++c)
  {
    if (c != '*')
    {
      AddTransition(2, c, 2);
      if (c != '/')
        AddTransition(3, c, 2);
    }
  }
  AddTransition(2, '\n', 2);
  AddTransition(2, '\t', 2);
  AddTransition(3, '\n', 2);
  AddTransition(3, '\t', 2);

  AddTransition(2, '*', 3);
  AddTransition(3, '*', 3);
  AddTransition(3, '/', 4);

  // - Transition Rules for Multi line comments
  AddTransition(1, '/', 5);

  for (char_type c = ' '; c <= '~'; ++c)
  {
    if (c != '\n')
      AddTransition(5, c, 5);
  }
  AddTransition(5, '\t', 5);

  // - Transition Rules for Character literals
  AddTransition(0, '\'', 6);
  AddTransition(6, '\\', 7);
  AddTransition(7, '\\', 6);
  AddTransition(7, 'n', 6);
  AddTransition(7, 't', 6);
  AddTransition(7, '\"', 6);
  AddTransition(7, '\'', 6);

  for (char_type c = ' '; c <= '~'; ++c)
  {
    if (c != '\'' && c != '\\')
    {
      AddTransition(6, c, 6);
    }
  }
  AddTransition(6, '\'', 8);

  // - Transition Rules for String literals
  AddTransition(0, '\"', 9);
  AddTransition(9, '\\', 10);
  AddTransition(10, '\\', 9);
  AddTransition(10, 'n', 9);
  AddTransition(10, 't', 9);
  AddTransition(10, '\"', 9);
  AddTransition(10, '\'', 9);

  for (char_type c = ' '; c <= '~'; ++c)
  {
    if (c != '\"' && c != '\\')
      AddTransition(9, c, 9);
  }
  AddTransition(9, '\"', 11);

  // - Transition Rules for Space, tab, crlf literals
  AddTransition(0, ' ', 12);
  AddTransition(0, '\t', 12);
  AddTransition(0, '\n', 12);
  AddTransition(12, ' ', 12);
  AddTransition(12, '\t', 12);
  AddTransition(12, '\n', 12);

  // - Transition Rules for Identifier, keyword, boolean
  AddTransition(0, '_', 13);
  AddTransition(0, '@', 13);
  for (char_type c = 'a'; c <= 'z'; ++c)
  {
    AddTransition(0, c, 13);                                    // lower case
    AddTransition(0, c - 32, 13);                               // upper case

    AddTransition(13, c, 13);                                   // lower case
    AddTransition(13, c - 32, 13);                              // upper case
  }

  for (char_type c = '0'; c <= '9'; ++c)
    AddTransition(13, c, 13);

  AddTransition(13, '_', 13);
  AddTransition(13, '@', 13);

  // - Transition Rules for Integer, floating point literals
  for (char_type c = '0'; c <= '9'; ++c)
  {
    AddTransition(0, c, 14);
    AddTransition(14, c, 14);
    AddTransition(15, c, 15);
    AddTransition(17, c, 18);
    AddTransition(18, c, 18);
  }
  AddTransition(14, '.', 15);
  AddTransition(15, 'e', 16);
  AddTransition(15, 'E', 16);
  AddTransition(16, '+', 17);
  AddTransition(16, '-', 17);
  AddTransition(18, 'f', 19);
  AddTransition(18, 'F', 19);

  // - Transition Rules for separators
  AddTransition(0, '.', 20);
  AddTransition(0, ',', 20);
  AddTransition(0, ';', 20);
  AddTransition(0, '?', 20);
  AddTransition(0, ':', 20);

  // - Transition Rules for brackets
  AddTransition(0, '{', 21);
  AddTransition(0, '}', 21);
  AddTransition(0, '[', 21);
  AddTransition(0, ']', 21);
  AddTransition(0, '(', 21);
  AddTransition(0, ')', 21);

  // - Transition Rules for operators


  AddTransition(0, '+', 22);                                      // +, ++, +=
  AddTransition(22, '+', 23);
  AddTransition(22, '=', 23);

  AddTransition(0, '-', 24);                                      // -, --, -=
  AddTransition(24, '-', 25);
  AddTransition(24, '=', 25);

  AddTransition(0, '*', 26);                                      // *, *=
  AddTransition(26, '=', 27);

  AddTransition(0, '^', 28);                                      // ^, ^=
  AddTransition(28, '=', 29);

  AddTransition(0, '/', 30);                                      // /, /=
  AddTransition(30, '=', 31);

  AddTransition(0, '&', 32);                                      // &, &&, &=
  AddTransition(32, '&', 33);
  AddTransition(32, '=', 33);

  AddTransition(0, '|', 34);                                      // |, ||, |=
  AddTransition(34, '|', 35);
  AddTransition(34, '=', 35);

  AddTransition(0, '=', 36);                                      // =, ==
  AddTransition(36, '=', 37);

  AddTransition(0, '%', 38);                                      // %, %=
  AddTransition(38, '=', 39);

  AddTransition(0, '!', 40);                                      // !, !=
  AddTransition(40, '=', 41);

  AddTransition(0, '~', 42);                                      // ~

  AddTransition(0, '>', 43);                                      // >, >>, >=, >>=, >>>, >>>=
  AddTransition(43, '>', 44);
  AddTransition(43, '=', 45);
  AddTransition(44, '>', 46);
  AddTransition(44, '=', 47);
  AddTransition(46, '=', 48);

  AddTransition(0, '<', 49);                                      // <, <<, <=
  AddTransition(49, '<', 50);
  AddTransition(49, '=', 51);
  AddTransition(50, '=', 52);
}


Token<Lexer::char_type> Lexer::GetToken()
{
  std::ostringstream stored_sequence;

  states_type new_state(initial_state_q0_);                                                           // reset the automata to the initial state
  delta_state_transition crt_transition;                                                              // the current transition
  delta_transition_states_mapping::const_iterator my_crt_position(delta_states_mapping_qd_.begin());  // iterators for my transitions table
  delta_transition_states_mapping::const_iterator states_mapping_end(delta_states_mapping_qd_.end());

  char_type crt_character = '\0', peek_char = '\0';
  bool bStateMachineLocked = false;

  crt_transition.first = initial_state_q0_;

  while ((crt_character = crt_stream_.get()) != EOF)
  {
    ++crt_position_;

    crt_transition.second = crt_character;

    my_crt_position = delta_states_mapping_qd_.find(crt_transition);    // find the current state from the crt_transition
    bStateMachineLocked = (my_crt_position == states_mapping_end);

    if (!bStateMachineLocked)                                           // I'm trying to identify the longest token so keep working until I'm locked
    {
      stored_sequence << crt_character;

      new_state = my_crt_position->second;                              // this is the new state -> switch to the new state

      if ((peek_char = crt_stream_.peek()) != EOF)                      // reached the stream's end ?
      {
        crt_transition.first = new_state;
        crt_transition.second = peek_char;                              // next input from the sequence
      }
    }
    else                                                                // machine locked
    {
      crt_stream_.unget();                                              // put the last character back on the tape and break
      --crt_position_;
      break;
    }
  }

  std::map<states_type, states_label>::const_iterator final_state;
  IdentifierCategory crt_category, new_category;
  final_state = final_states_set_f_.find(new_state);                    // check if my last state can be found in my set of final states

  if (final_state != final_states_set_f_.end())
  {
    string_type token_value = stored_sequence.str();
    crt_category = static_cast<IdentifierCategory>(final_state->second);

    if (final_state->second == CHECK_MULTI_STATE)                       // search multi state elements equivalence into the language keywords
    {
      if (grammar_symbols_->HasIdentifier(token_value, new_category))
        crt_category = new_category;
      else
        crt_category = IDENTIFIER;
    }

    return Token<char_type>(*tokens_string_table_, token_value, crt_category);
  }
  else
    return Token<char_type>(*tokens_string_table_, "Error", UNRECOGNIZED_ERROR);
}

LEXER_END

