#ifndef _token_h_
#define _token_h_

#include "common.h"

AUTOMATON_START
DFA_START

template <class char_type = char>
class Token
{
  // - Types

  typedef char_type crt_char_type;
  enum TokenType : unsigned short { KEYWORD = 0x01, 
                                    INTEGER_LITERAL, FLOATING_POINT_LITERAL, BOOLEAN_LITERAL,
                                    CHARACTER_LITERAL, STRING_LITERAL, OPERATORS, WHITE_SPACE, 
                                    INLINE_COMMENT, MULTI_LINE_COMMENT, 
                                    IDENTIFIER, SEPARATOR, PREDEFINED_TYPE };

public :




};

















DFA_END
AUTOMATON_END


#endif
