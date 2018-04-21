#ifndef _lexer_h_
#define _lexer_h_

#include <iostream>
#include <memory>

#include "DFA.hpp"
#include "token.hpp"

LEXER_START

template <class char_type = char>
class Lexer
{
  
  static_assert(lexer::tools::is_character<char_type>::value, "The class template can only be instantiated with char types like char/wchar_t");

public :
  // - Types

  typedef typename lexer::tools::if_is_of_type<char_type, wchar_t, 
                                               std::wistream, std::istream>::ret_type stream_type;  ///< stream type used for either ASCII or unicode files

public :

  /// Constructor
  Lexer(const stream_type& input_stream) : crt_stream(input_stream), crt_position(0),
                                           finite_state_machine_(new DFA<char_type, unsigned short>),
                                           tokens_string_table_(new TokenStringTable<char_type>)
  {
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
};

LEXER_END

#endif // _lexer_h_
