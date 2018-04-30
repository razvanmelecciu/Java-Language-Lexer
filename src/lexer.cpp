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
  AddFinalState(53, INTEGER_LITERAL);
  AddFinalState(55, INTEGER_LITERAL);
  AddFinalState(56, SEPARATOR);
  AddFinalState(57, INTEGER_LITERAL);

  // - Transition Rules for Single line comments
  AddTransition(0, '/', 1);
  AddTransition(1, '*', 2);
  AddTransition(2, all_characters, sizeof(all_characters) - 1, "*", 1, 2);
  AddTransition(3, all_characters, sizeof(all_characters) - 1, "*/", 2, 2);

  AddTransition(2, "\n\t", 2, 2);
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
  AddTransition(0, '0', 53);
  AddTransition(53, digits, sizeof(digits) - 1, 14);
  AddTransition(53, '.', 15);
  AddTransition(53, "xX", 2, 54);
  AddTransition(54, letters_hexa, sizeof(letters_hexa) - 1, 55);
  AddTransition(54, digits, sizeof(digits) - 1, 55);
  AddTransition(55, digits, sizeof(digits) - 1, 55);
  AddTransition(55, letters_hexa, sizeof(letters_hexa) - 1, 55);
  AddTransition(55, "lL", 2, 57);
  AddTransition(0, "123456789", 9, 14);
  AddTransition(14, digits, sizeof(digits) - 1, 14);
  AddTransition(15, digits, sizeof(digits) - 1, 15);
  AddTransition(16, digits, sizeof(digits) - 1, 18);
  AddTransition(17, digits, sizeof(digits) - 1, 18);
  AddTransition(18, digits, sizeof(digits) - 1, 18);
  AddTransition(14, '.', 15);
  AddTransition(14, "lL", 2, 57);
  AddTransition(15, "eE", 2, 16);
  AddTransition(16, "+-", 2, 17);
  AddTransition(18, "fF", 2, 19);

  // - Transition Rules for the Dot - special case
  AddTransition(0, '.', 56);
  AddTransition(56, digits, sizeof(digits) - 1, 15);

  // - Transition Rules for separators (The dot is a special case treated above)
  AddTransition(0, ",;?:", 4, 20);

  // - Transition Rules for brackets
  AddTransition(0, "{}[]()", 6, 21);

  // - Transition Rules for operators
  AddTransition(0, '+', 22);                                      // +, ++, +=
  AddTransition(22, "+=", 2, 23);

  AddTransition(0, '-', 24);                                      // -, --, -=
  AddTransition(24, "-=", 2, 25);

  AddTransition(0, '*', 26);                                      // *, *=
  AddTransition(26, '=', 27);

  AddTransition(0, '^', 28);                                      // ^, ^=
  AddTransition(28, '=', 29);

  AddTransition(0, '/', 30);                                      // /, /=
  AddTransition(30, '=', 31);

  AddTransition(0, '&', 32);                                      // &, &&, &=
  AddTransition(32, "&=", 2, 33);

  AddTransition(0, '|', 34);                                      // |, ||, |=
  AddTransition(34, "|=", 2, 35);

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

