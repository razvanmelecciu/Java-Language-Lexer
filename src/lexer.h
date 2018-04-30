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
  typedef typename tools::if_is_of_type<char_type, wchar_t, 
                                        std::wistream&, std::istream&>::ret_type in_stream_type;      ///< input stream type used for either ASCII or unicode files
  typedef typename tools::if_is_of_type<char_type, wchar_t,
                                        std::wostream&, std::ostream&>::ret_type out_stream_type;     ///< stream type used for either ASCII or unicode files
  typedef typename tools::if_is_of_type<char_type, wchar_t,
                                        std::wstring, std::string>::ret_type string_type;             ///< compile time type identification

public :

  /// Constructor
  Lexer(const in_stream_type& input_stream, const GrammarIdentifiers<char_type>& grammar_identifiers);
  /// Dtor
  ~Lexer();

  /// Copy ctor disabled
  Lexer(const Lexer& RHS) = delete;

  /// Copy assignment disabled
  Lexer& operator = (const Lexer& RHS) = delete;

  // - Accessors

  /// Get the file position
  std::size_t GetStreamPosition() const;
  /// Gets the next available token from the specified stream object (or a list of tokens if an err is encountered)
  Token<char_type> GetToken();

private:

  std::size_t    crt_position_;                                           ///< the position in the stream (current character processed)
  in_stream_type    crt_stream_;                                          ///< the input stream that will be tokenized
  std::unique_ptr<TokenStringTable<char_type> >    tokens_string_table_;  ///< the string table for the tokens identified
  std::unique_ptr<GrammarIdentifiers<char_type> >  grammar_symbols_;      ///< the table of keywords for the grammar
};

inline Lexer::~Lexer()
{
  // no manual stuff here...
}

inline std::size_t Lexer::GetStreamPosition() const
{
  return crt_position_;
}



LEXER_END

#endif // _lexer_h_
