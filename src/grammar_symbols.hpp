#ifndef _grammar_symbols_h_
#define _grammar_symbols_h_

#include "common.h"

LEXER_START

enum SymbolCategory : unsigned short
{
  KEYWORD = 0x01,
  INTEGER_LITERAL, FLOATING_POINT_LITERAL, BOOLEAN_LITERAL,
  CHARACTER_LITERAL, STRING_LITERAL, OPERATORS, WHITE_SPACE,
  INLINE_COMMENT, MULTI_LINE_COMMENT, BRACKETS,
  IDENTIFIER, SEPARATOR, PREDEFINED_TYPE
};

template <typename char_type = char>
struct GrammarSymbols
{



/*private :
  std::map<std::string, SymbolCategory>*/
};

LEXER_END

#endif // _grammar_symbols_h_

