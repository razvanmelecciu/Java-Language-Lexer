#ifndef _lexer_h_
#define _lexer_h_

#include <iostream>
#include <sstream>
#include <memory>
#include <list>

#include "DFA.hpp"
#include "token.hpp"
#include "grammar_symbols.hpp"

LEXER_START

class Lexer : public DFA<char, unsigned short, unsigned short>
{
public :
  // - Types

  typedef char char_type;                                                                             ///< the current character type
  enum { CHECK_MULTI_STATE = 0x00 };                                                                  ///< used to further check against the grammar symbols
  typedef typename lexer::tools::if_is_of_type<char_type, wchar_t, 
                                               std::wistream&, std::istream&>::ret_type stream_type;  ///< stream type used for either ASCII or unicode files
  typedef typename lexer::tools::if_is_of_type<char_type, wchar_t,
                                               std::wstring, std::string>::ret_type string_type;      ///< compile time type identification
  typedef std::list<Token<char_type> > token_list;                                                    ///< list of tokens

public :

  /// Constructor
  Lexer(const stream_type& input_stream, 
        const GrammarIdentifiers<char_type>& grammar_identifiers) : crt_stream_(input_stream), 
                                                                    grammar_symbols_(new GrammarIdentifiers<char_type>), 
                                                                    crt_position_(0),
                                                                    tokens_string_table_(new TokenStringTable<char_type>)
  {
    // - Initialize the DFA with some generic rules

    // - Initial state
    SetInitialState(0);

    // - Final states with labels
    AddFinalState(1,  CHECK_MULTI_STATE);
    AddFinalState(5,  INLINE_COMMENT);
    AddFinalState(4,  MULTI_LINE_COMMENT);
    AddFinalState(7,  CHARACTER_LITERAL);
    AddFinalState(9,  CHARACTER_LITERAL);
    AddFinalState(11, STRING_LITERAL);
    AddFinalState(13, STRING_LITERAL);
    AddFinalState(14, WHITE_SPACE);
    AddFinalState(15, CHECK_MULTI_STATE);
    AddFinalState(16, INTEGER_LITERAL);
    AddFinalState(17, FLOATING_POINT_LITERAL);
    AddFinalState(20, FLOATING_POINT_LITERAL);
    AddFinalState(21, FLOATING_POINT_LITERAL);

    // - Start adding the transition rule

    // - Rules for Single line comments
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

    // - Rules for Multi line comments
    AddTransition(1, '/', 5);        

    for (char_type c = ' '; c <= '~'; ++c)
    {
      if (c != '\n')
        AddTransition(5, c, 5);
    }
    AddTransition(5, '\t', 5);

    // - Rules for Character literals
    AddTransition(0, '\'', 6);      
    AddTransition(6, '\'', 7);

    for (char_type c = ' '; c <= '~'; ++c)
    {
      if (c != '\'')
      {
        AddTransition(6, c, 8);
        AddTransition(8, c, 8);
      }
    }
    AddTransition(6, '\n', 8);
    AddTransition(6, '\t', 8);
    AddTransition(8, '\n', 8);
    AddTransition(8, '\t', 8);
    AddTransition(8, '\'', 9);

    // - Rules for String literals
    AddTransition(0, '\"', 10);      
    AddTransition(10, '\"', 11);

    for (char_type c = ' '; c <= '~'; ++c)
    {
      if (c != '\"')
      {
        AddTransition(10, c, 12);
        AddTransition(12, c, 12);
      }
    }
    AddTransition(10, '\n', 12);
    AddTransition(10, '\t', 12);
    AddTransition(12, '\n', 12);
    AddTransition(12, '\t', 12);
    AddTransition(12, '\'', 13);

    // - Rules for Space, tab, crlf literals
    AddTransition(0, ' ', 14);     
    AddTransition(0, '\t', 14);    
    AddTransition(0, '\n', 14);
    AddTransition(14, ' ', 14);
    AddTransition(14, '\t', 14);    
    AddTransition(14, '\n', 14);

    // - Rules for Integer, floating point literals
    for (char_type c = '0'; c <= '9'; ++c)
    {
      AddTransition(0, c, 16);
      AddTransition(16, c, 16);
      AddTransition(17, c, 17);
      AddTransition(19, c, 20);
      AddTransition(20, c, 20);
    }
    AddTransition(16, '.', 17);
    AddTransition(17, 'e', 18);
    AddTransition(17, 'E', 18);
    AddTransition(18, '+', 19);
    AddTransition(18, '-', 19);
    AddTransition(20, 'f', 21);
    AddTransition(20, 'F', 21);

    // - Rules for Identifier, keyword, boolean, operator, separator literals
    AddTransition(0, '_', 15);    
    for (char_type c = 'a'; c <= 'z'; ++c)
    {
      AddTransition(0, c, 15);                                    // lower case
      AddTransition(0, c - 32, 15);                               // upper case
                                                                  
      AddTransition(15, c, 15);                                   // lower case
      AddTransition(15, c - 32, 15);                              // upper case
    }

    for (char_type c = '0'; c <= '9'; ++c)
      AddTransition(15, c, 15);

    AddTransition(15, '_', 15);
  }

  /// Dtor
  ~Lexer()
  {
    // no manual stuff here...
  }

  /// Copy ctor disabled
  Lexer(const Lexer& RHS) = delete;

  /// Copy assignment disabled
  Lexer& operator = (const Lexer& RHS) = delete;


  // - Accessors

  /// Get the file position
  std::size_t GetStreamPosition() const
  {
    return crt_position_;
  }

  /// Gets the next available token from the specified stream object (or a list of tokens if an err is encountered)
  Messages GetToken(token_list& resulting_tokens)
  {
    resulting_tokens.clear();

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

      Token<char_type> my_token(*tokens_string_table_, token_value, crt_category);
      resulting_tokens.push_back(my_token);
      return Messages::RECOGNIZED;
    }
    else
    {
      return Messages::UNRECOGNIZED;
    }
  }

private:

  std::size_t    crt_position_;                                           ///< the position in the stream (current character processed)
  stream_type    crt_stream_;                                             ///< the input stream that will be tokenized
  std::unique_ptr<TokenStringTable<char_type> >    tokens_string_table_;  ///< the string table for the tokens identified
  std::unique_ptr<GrammarIdentifiers<char_type> >  grammar_symbols_;      ///< the table of keywords for the grammar
};

LEXER_END

#endif // _lexer_h_
