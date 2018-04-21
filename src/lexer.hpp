#ifndef _lexer_h_
#define _lexer_h_

#include "DFA.hpp"
#include "token.hpp"

LEXER_START

template <class char_type = char>
class Lexer
{

  static_assert(lexer::tools::is_character<char_type>::value, "The class template can only be instantiated with char types like char/wchar_t");

  // - Types

public :

  typedef 


public :
  Lexer()





private:

  DFA<char_type, unsigned short> det_finite_automata;
};

LEXER_END

#endif // _lexer_h_
