#ifndef _grammar_symbols_h_
#define _grammar_symbols_h_

#include "common.h"
#include "tools.hpp"

#include <map>
#include <string>

LEXER_START

enum IdentifierCategory : unsigned short
{
  KEYWORD = 0x01, IDENTIFIER, BOOLEAN_LITERAL, PREDEFINED_TYPE,
  INTEGER_LITERAL, FLOATING_POINT_LITERAL,
  CHARACTER_LITERAL, STRING_LITERAL, 
  OPERATOR, WHITE_SPACE,
  INLINE_COMMENT, MULTI_LINE_COMMENT, BRACKET,
  SEPARATOR
};

template <typename char_type>
struct GrammarIdentifiers
{
  static_assert(lexer::tools::is_character<char_type>::value, "The class template can only be instantiated with char types like char/wchar_t");
};

template <>
struct GrammarIdentifiers<char>
{
  typedef char character_type;
  typedef std::string crt_string_type;
  typedef std::map<crt_string_type, IdentifierCategory> string_table;

  GrammarIdentifiers()
  {
    // - keywords for this grammar
    grammar_symbols_.insert(string_table::value_type("if", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("else", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("for", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("while", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("do", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("try", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("catch", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("throw", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("throws", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("finally", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("switch", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("case", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("default", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("break", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("continue", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("return", KEYWORD));

    grammar_symbols_.insert(string_table::value_type("main", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("synchronized", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("volatile", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("transient", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("strictfp", KEYWORD));

    grammar_symbols_.insert(string_table::value_type("package", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("import", KEYWORD));

    grammar_symbols_.insert(string_table::value_type("enum", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("class", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("interface", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("implements", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("extends", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("abstract", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("super", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("this", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("instanceof", KEYWORD));

    grammar_symbols_.insert(string_table::value_type("native", KEYWORD));

    grammar_symbols_.insert(string_table::value_type("public", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("private", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("protected", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("static", KEYWORD));

    grammar_symbols_.insert(string_table::value_type("final", KEYWORD));
    grammar_symbols_.insert(string_table::value_type("const", KEYWORD));

    grammar_symbols_.insert(string_table::value_type("new", KEYWORD));

    // - predefined types
    grammar_symbols_.insert(string_table::value_type("void", PREDEFINED_TYPE));
    grammar_symbols_.insert(string_table::value_type("char", PREDEFINED_TYPE));
    grammar_symbols_.insert(string_table::value_type("byte", PREDEFINED_TYPE));
    grammar_symbols_.insert(string_table::value_type("boolean", PREDEFINED_TYPE));
    grammar_symbols_.insert(string_table::value_type("short", PREDEFINED_TYPE));
    grammar_symbols_.insert(string_table::value_type("int", PREDEFINED_TYPE));
    grammar_symbols_.insert(string_table::value_type("long", PREDEFINED_TYPE));
    grammar_symbols_.insert(string_table::value_type("float", PREDEFINED_TYPE));
    grammar_symbols_.insert(string_table::value_type("double", PREDEFINED_TYPE));
    grammar_symbols_.insert(string_table::value_type("null", PREDEFINED_TYPE));
    grammar_symbols_.insert(string_table::value_type("String", PREDEFINED_TYPE));

    // - boolean literals
    grammar_symbols_.insert(string_table::value_type("true", BOOLEAN_LITERAL));
    grammar_symbols_.insert(string_table::value_type("false", BOOLEAN_LITERAL));
  }

  bool HasIdentifier(const crt_string_type& identifier, IdentifierCategory& category)
  {
    string_table::const_iterator crt_item = grammar_symbols_.find(identifier);
    if (crt_item != grammar_symbols_.end())
    {
      category = crt_item->second;
      return true;
    }

    return false;
  }

  // - Members

public :

  string_table grammar_symbols_;
};


template <>
struct GrammarIdentifiers<wchar_t>
{
  typedef wchar_t character_type;
  typedef std::wstring crt_string_type;
  typedef std::map<crt_string_type, IdentifierCategory> string_table;

  GrammarIdentifiers()
  {
    // - keywords for this grammar
    grammar_symbols_.insert(string_table::value_type(L"if", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"else", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"for", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"while", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"do", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"try", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"catch", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"throw", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"throws", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"finally", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"switch", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"case", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"default", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"break", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"continue", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"return", KEYWORD));

    grammar_symbols_.insert(string_table::value_type(L"main", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"synchronized", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"volatile", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"transient", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"strictfp", KEYWORD));

    grammar_symbols_.insert(string_table::value_type(L"package", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"import", KEYWORD));

    grammar_symbols_.insert(string_table::value_type(L"enum", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"class", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"interface", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"implements", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"extends", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"abstract", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"super", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"this", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"instanceof", KEYWORD));

    grammar_symbols_.insert(string_table::value_type(L"native", KEYWORD));

    grammar_symbols_.insert(string_table::value_type(L"public", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"private", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"protected", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"static", KEYWORD));

    grammar_symbols_.insert(string_table::value_type(L"final", KEYWORD));
    grammar_symbols_.insert(string_table::value_type(L"const", KEYWORD));

    grammar_symbols_.insert(string_table::value_type(L"new", KEYWORD));

    // - predefined types
    grammar_symbols_.insert(string_table::value_type(L"void", PREDEFINED_TYPE));
    grammar_symbols_.insert(string_table::value_type(L"char", PREDEFINED_TYPE));
    grammar_symbols_.insert(string_table::value_type(L"byte", PREDEFINED_TYPE));
    grammar_symbols_.insert(string_table::value_type(L"boolean", PREDEFINED_TYPE));
    grammar_symbols_.insert(string_table::value_type(L"short", PREDEFINED_TYPE));
    grammar_symbols_.insert(string_table::value_type(L"int", PREDEFINED_TYPE));
    grammar_symbols_.insert(string_table::value_type(L"long", PREDEFINED_TYPE));
    grammar_symbols_.insert(string_table::value_type(L"float", PREDEFINED_TYPE));
    grammar_symbols_.insert(string_table::value_type(L"double", PREDEFINED_TYPE));
    grammar_symbols_.insert(string_table::value_type(L"null", PREDEFINED_TYPE));
    grammar_symbols_.insert(string_table::value_type(L"String", PREDEFINED_TYPE));

    // - boolean literals
    grammar_symbols_.insert(string_table::value_type(L"true", BOOLEAN_LITERAL));
    grammar_symbols_.insert(string_table::value_type(L"false", BOOLEAN_LITERAL));
  }

public :

  string_table grammar_symbols_;
};

LEXER_END

#endif // _grammar_symbols_h_

