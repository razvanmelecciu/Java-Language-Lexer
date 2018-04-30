#include "stdafx.h"
#include "lexer.h"

LEXER_START

Lexer::Lexer(const in_stream_type& input_stream,
             const GrammarIdentifiers<char_type>& grammar_identifiers) : crt_stream_(input_stream),
                                                                         grammar_symbols_(new GrammarIdentifiers<char_type>), crt_position_(0),
                                                                         tokens_string_table_(new TokenStringTable<char_type>)
{
  const char letters_lwr[]       = "abcdefghijklmnopqrstuvwxyz";
  const char letters_upr[]       = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  const char letters_hexa[]      = "abcdefABCDEF";
  const char digits[]            = "0123456789";
  const char special_var_chars[] = "_$@";
  const char all_characters[]    = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
  const char tabs_spaces[]       = " \n\t";

  // - Initialize the DFA with the scanning rules

  // - Initial state
  SetInitialState(0);

  // - Final states with labels
  AddFinalState(1, OPERATOR);
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
  AddFinalState(20, INTEGER_LITERAL);
  AddFinalState(22, INTEGER_LITERAL);
  AddFinalState(23, INTEGER_LITERAL);
  AddFinalState(24, SEPARATOR);
  AddFinalState(25, SEPARATOR);
  AddFinalState(26, BRACKET);
  for (short i = 27; i <= 56; ++i)
      AddFinalState(i, OPERATOR);

  // - Transition Rules for Single line comments and / along with /= operators
  AddTransition(0, '/', 1);
  AddTransition(1, '*', 2);
  AddTransition(1, '=', 35);
  AddTransition(2, all_characters, sizeof(all_characters) - 1, "*", 1, 2);
  AddTransition(2, "\n\t", 2, 2);
  AddTransition(3, all_characters, sizeof(all_characters) - 1, "*/", 2, 2);
  AddTransition(3, "\n\t", 2, 2);
  AddTransition(2, '*', 3);
  AddTransition(3, '*', 3);
  AddTransition(3, '/', 4);

  // - Transition Rules for Multi line comments
  AddTransition(1, '/', 5);
  AddTransition(5, all_characters, sizeof(all_characters) - 1, "\n", 1, 5);
  AddTransition(5, '\t', 5);

  // - Transition Rules for Character literals
  AddTransition(0, '\'', 6);
  AddTransition(6, '\\', 7);
  AddTransition(7, "\\nt\"\'", 5, 6);
  AddTransition(6, all_characters, sizeof(all_characters) - 1, "\'\\", 2, 6);
  AddTransition(6, '\'', 8);

  // - Transition Rules for String literals
  AddTransition(0, '\"', 9);
  AddTransition(9, '\\', 10);
  AddTransition(10, "\\nt\"\'", 5, 9);
  AddTransition(9, all_characters, sizeof(all_characters) - 1, "\"\\", 2, 9);
  AddTransition(9, '\"', 11);

  // - Transition Rules for Space, tab, crlf literals
  AddTransition(0, tabs_spaces, sizeof(tabs_spaces) - 1, 12);
  AddTransition(12, tabs_spaces, sizeof(tabs_spaces) - 1, 12);

  // - Transition Rules for Identifier, keyword, boolean
  AddTransition(0, special_var_chars, sizeof(special_var_chars) - 1, 13);
  AddTransition(0, letters_lwr, sizeof(letters_lwr) - 1, 13);
  AddTransition(0, letters_upr, sizeof(letters_upr) - 1, 13);
  AddTransition(13, letters_lwr, sizeof(letters_lwr) - 1, 13);
  AddTransition(13, letters_upr, sizeof(letters_upr) - 1, 13);
  AddTransition(13, digits, sizeof(digits) - 1, 13);
  AddTransition(13, special_var_chars, sizeof(special_var_chars) - 1, 13);

  // - Transition Rules for Integer, floating point literals
  AddTransition(0, "123456789", 9, 14);
  AddTransition(14, digits, sizeof(digits) - 1, 14);
  AddTransition(15, digits, sizeof(digits) - 1, 15);
  AddTransition(16, digits, sizeof(digits) - 1, 18);
  AddTransition(17, digits, sizeof(digits) - 1, 18);
  AddTransition(18, digits, sizeof(digits) - 1, 18);
  AddTransition(14, '.', 15);
  AddTransition(14, "lL", 2, 23);
  AddTransition(14, "eE", 2, 16);
  AddTransition(15, "eE", 2, 16);
  AddTransition(16, "+-", 2, 17);
  AddTransition(18, "fF", 2, 19);
  AddTransition(0, '0', 20);
  AddTransition(20, digits, sizeof(digits) - 1, 14);
  AddTransition(20, '.', 15);
  AddTransition(20, "xX", 2, 21);
  AddTransition(21, letters_hexa, sizeof(letters_hexa) - 1, 22);
  AddTransition(21, digits, sizeof(digits) - 1, 22);
  AddTransition(22, letters_hexa, sizeof(letters_hexa) - 1, 22);
  AddTransition(22, digits, sizeof(digits) - 1, 22);
  AddTransition(22, "lL", 2, 23);

  // - Transition Rules for the Dot - special case
  AddTransition(0, '.', 25);
  AddTransition(25, digits, sizeof(digits) - 1, 15);

  // - Transition Rules for separators (The dot is a special case treated above)
  AddTransition(0, ",;?:", 4, 24);

  // - Transition Rules for brackets
  AddTransition(0, "{}[]()", 6, 26);

  // - Transition Rules for operators
  AddTransition(0, '+', 27);                                      // +, ++, +=
  AddTransition(27, "+=", 2, 28);

  AddTransition(0, '-', 29);                                      // -, --, -=
  AddTransition(29, "-=", 2, 30);

  AddTransition(0, '*', 31);                                      // *, *=
  AddTransition(31, '=', 32);

  AddTransition(0, '^', 33);                                      // ^, ^=
  AddTransition(33, '=', 34);

  AddTransition(0, '&', 36);                                      // &, &&, &=
  AddTransition(36, "&=", 2, 37);

  AddTransition(0, '|', 38);                                      // |, ||, |=
  AddTransition(38, "|=", 2, 39);

  AddTransition(0, '=', 40);                                      // =, ==
  AddTransition(40, '=', 41);

  AddTransition(0, '%', 42);                                      // %, %=
  AddTransition(42, '=', 43);

  AddTransition(0, '!', 44);                                      // !, !=
  AddTransition(44, '=', 45);

  AddTransition(0, '~', 46);                                      // ~

  AddTransition(0, '>', 47);                                      // >, >>, >=, >>=, >>>, >>>=
  AddTransition(47, '>', 48);
  AddTransition(47, '=', 49);
  AddTransition(48, '>', 50);
  AddTransition(48, '=', 51);
  AddTransition(50, '=', 52);

  AddTransition(0, '<', 53);                                      // <, <<, <=
  AddTransition(53, '<', 54);
  AddTransition(53, '=', 55);
  AddTransition(54, '=', 56);
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

