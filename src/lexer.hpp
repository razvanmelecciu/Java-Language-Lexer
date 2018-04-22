#ifndef _lexer_h_
#define _lexer_h_

#include <iostream>
#include <memory>

#include "DFA.hpp"
#include "token.hpp"
#include "grammar_symbols.hpp"

LEXER_START

class Lexer
{
public :
  // - Types

  typedef char char_type;
  enum { CHECK_MULTI_STATE = 0x00 };                                                                  ///< used to further check against the grammar symbols
  typedef typename lexer::tools::if_is_of_type<char_type, wchar_t, 
                                               std::wistream&, std::istream&>::ret_type stream_type;  ///< stream type used for either ASCII or unicode files

public :

  /// Constructor
  Lexer(const stream_type& input_stream, 
        const GrammarIdentifiers<char_type>& grammar_identifiers) : crt_stream_(input_stream), grammar_symbols_(new GrammarIdentifiers<char_type>), 
                                                                    crt_position_(0),
                                                                    finite_state_machine_(new DFA<char_type, unsigned short, unsigned short>),
                                                                    tokens_string_table_(new TokenStringTable<char_type>)
  {
    // - Initial state
    finite_state_machine_->SetInitialState(0);

    // - DFA final states with labels
    finite_state_machine_->AddFinalState(1,  CHECK_MULTI_STATE);
    finite_state_machine_->AddFinalState(5,  INLINE_COMMENT);
    finite_state_machine_->AddFinalState(4,  MULTI_LINE_COMMENT);
    finite_state_machine_->AddFinalState(7,  CHARACTER_LITERAL);
    finite_state_machine_->AddFinalState(9,  CHARACTER_LITERAL);
    finite_state_machine_->AddFinalState(11, STRING_LITERAL);
    finite_state_machine_->AddFinalState(13, STRING_LITERAL);
    finite_state_machine_->AddFinalState(14, WHITE_SPACE);
    finite_state_machine_->AddFinalState(15, CHECK_MULTI_STATE);
    finite_state_machine_->AddFinalState(16, INTEGER_LITERAL);
    finite_state_machine_->AddFinalState(17, FLOATING_POINT_LITERAL);
    finite_state_machine_->AddFinalState(20, FLOATING_POINT_LITERAL);
    finite_state_machine_->AddFinalState(21, FLOATING_POINT_LITERAL);

    // - Start adding the transition rules
    finite_state_machine_->AddTransition(0, '/', 1);       // single line comment
    finite_state_machine_->AddTransition(1, '*', 2);

    for (char c = ' '; c <= '~'; ++c)
    {
      if (c != '*')
      {
        finite_state_machine_->AddTransition(2, c, 2);
        finite_state_machine_->AddTransition(3, c, 2);
      }
    }
    finite_state_machine_->AddTransition(2, '\n', 2);
    finite_state_machine_->AddTransition(2, '\t', 2);
    finite_state_machine_->AddTransition(3, '\n', 2);
    finite_state_machine_->AddTransition(3, '\t', 2);

    finite_state_machine_->AddTransition(2, '*', 3);
    finite_state_machine_->AddTransition(3, '*', 3);
    finite_state_machine_->AddTransition(3, '/', 4);

    finite_state_machine_->AddTransition(1, '/', 5);       // multi line comment 

    for (char c = ' '; c <= '~'; ++c)
    {
      if (c != '\n')
        finite_state_machine_->AddTransition(5, c, 5);
    }
    finite_state_machine_->AddTransition(5, '\t', 5);

    finite_state_machine_->AddTransition(0, '\'', 6);      // character literal
    finite_state_machine_->AddTransition(6, '\'', 7);

    for (char c = ' '; c <= '~'; ++c)
    {
      if (c != '\'')
      {
        finite_state_machine_->AddTransition(6, c, 8);
        finite_state_machine_->AddTransition(8, c, 8);
      }
    }
    finite_state_machine_->AddTransition(6, '\n', 8);
    finite_state_machine_->AddTransition(6, '\t', 8);
    finite_state_machine_->AddTransition(8, '\n', 8);
    finite_state_machine_->AddTransition(8, '\t', 8);
    finite_state_machine_->AddTransition(8, '\'', 9);

    finite_state_machine_->AddTransition(0, '\"', 10);      // string literal
    finite_state_machine_->AddTransition(10, '\"', 11);

    for (char c = ' '; c <= '~'; ++c)
    {
      if (c != '\"')
      {
        finite_state_machine_->AddTransition(10, c, 12);
        finite_state_machine_->AddTransition(12, c, 12);
      }
    }
    finite_state_machine_->AddTransition(10, '\n', 12);
    finite_state_machine_->AddTransition(10, '\t', 12);
    finite_state_machine_->AddTransition(12, '\n', 12);
    finite_state_machine_->AddTransition(12, '\t', 12);
    finite_state_machine_->AddTransition(12, '\'', 13);

    finite_state_machine_->AddTransition(0, ' ', 14);     // space, tab, crlf literal
    finite_state_machine_->AddTransition(0, '\t', 14);    
    finite_state_machine_->AddTransition(0, '\n', 14);
    finite_state_machine_->AddTransition(14, ' ', 14);
    finite_state_machine_->AddTransition(14, '\t', 14);    
    finite_state_machine_->AddTransition(14, '\n', 14);

    for (char c = '0'; c <= '9'; ++c)                     // integer, floating point literal
    {
      finite_state_machine_->AddTransition(0, c, 16);
      finite_state_machine_->AddTransition(16, c, 16);
      finite_state_machine_->AddTransition(17, c, 17);
      finite_state_machine_->AddTransition(19, c, 20);
      finite_state_machine_->AddTransition(20, c, 20);
    }
    finite_state_machine_->AddTransition(16, '.', 17);
    finite_state_machine_->AddTransition(17, 'e', 18);
    finite_state_machine_->AddTransition(17, 'E', 18);
    finite_state_machine_->AddTransition(18, '+', 19);
    finite_state_machine_->AddTransition(18, '-', 19);
    finite_state_machine_->AddTransition(20, 'f', 21);
    finite_state_machine_->AddTransition(20, 'F', 21);

    finite_state_machine_->AddTransition(0, '_', 15);    // indetifier or keyword or boolean literal
    for (char c = 'a'; c <= 'z'; ++c)
    {
      finite_state_machine_->AddTransition(0, c, 15);                                    // lower case
      finite_state_machine_->AddTransition(0, c - 65, 15);                               // upper case
                                                                                         
      finite_state_machine_->AddTransition(15, c, 15);                                   // lower case
      finite_state_machine_->AddTransition(15, c - 65, 15);                              // upper case
    }

    for (char c = '0'; c <= '9'; ++c)                       
      finite_state_machine_->AddTransition(15, c, 15);

    finite_state_machine_->AddTransition(15, '_', 15);
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

  /// Gets the next available token from the specified stream object
  Token<char_type> GetToken()
  {

  }


private:

  std::size_t    crt_position_;          ///< the position in the stream (current character processed)
  stream_type    crt_stream_;            ///< the input stream that will be tokenized
  std::unique_ptr<DFA<char_type, unsigned short> > finite_state_machine_;  ///< the underlying finite state machine
  std::unique_ptr<TokenStringTable<char_type> >    tokens_string_table_;   ///< the string table for the tokens identified
  std::unique_ptr<GrammarIdentifiers<char_type> >      grammar_symbols_;       ///< the table of keywords for the grammar
};

LEXER_END

#endif // _lexer_h_
