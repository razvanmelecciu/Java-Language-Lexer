#ifndef _token_h_
#define _token_h_

#include "common.h"
#include "tools.hpp"
#include <string>

LEXER_START

enum TokenType : unsigned short
{
  KEYWORD = 0x01,
  INTEGER_LITERAL, FLOATING_POINT_LITERAL, BOOLEAN_LITERAL,
  CHARACTER_LITERAL, STRING_LITERAL, OPERATORS, WHITE_SPACE,
  INLINE_COMMENT, MULTI_LINE_COMMENT, BRACKETS,
  IDENTIFIER, SEPARATOR, PREDEFINED_TYPE
};

template <class char_type = char>
class Token
{
  static_assert(lexer::tools::is_character<char_type>::value, "The class template can only be instantiated with char types like char/wchar_t");

  // - Types

public:

  typedef char_type my_char_type;
  typedef typename lexer::tools::if_is_of_type<char_type, wchar_t, 
                                               std::wstring, std::string>::ret_type string_type;   ///< compile time type identification

public :

  /// Default Ctor 0
  Token() : type_(KEYWORD)
  {
  }

  /// Ctor 0
  explicit Token(TokenType type) : type_(type)
  {
  }

  /// Ctor 2
  Token(const char_type* value, std::size_t size, TokenType my_type) : type_(my_type)
  {
    value_.insert(size, value);
  }

  /// Ctor 3
  Token(const string_type& value, TokenType my_type) : type_(my_type), value_(value)
  {
  }

  /// Copy
  Token(const Token& RHS) : value_(RHS.value_), type_(RHS.type)
  {
  }

#if defined MOVE_SEMANTICS
  /// Move
  Token(Token&& RHS) : value_(static_cast<string_type&&>(RHS.value_)), type_(RHS.type)
  {
  }
#endif

  /// Dtor
  ~Token()
  {
  }

  // - Operators

  Token& operator = (const Token& RHS)
  {
    if (this != &RHS)
    {
      value_ = RHS.value_;
      type_  = RHS.value_;
    }
    return *this;
  }

#if defined MOVE_SEMANTICS
  Token& operator = (Token&& RHS)
  {
    if (this != &RHS)
    {
      value_ = RHS.value_;
      type_ = RHS.value_;
    }
    return *this;
  }
#endif

  // - Accessors

  /// Get the current type
  TokenType GetType() const
  {
    return type_;
  }

  /// Get the current value
  void GetValue(string_type& value) const
  {
    value.clear();
    value = value_;
  }

  /// Has value
  bool HasValue() const
  {
    bool has_value = value_.size() > 0;
    return has_value;
  }

  // - Mutators

  /// Set the type
  Token& SetType(TokenType type)
  {
    type_ = type;
    return *this;
  }

  /// Set the value
  Token& SetValue(const char_type* value, std::size_t size)
  {
    value_.clear();
    value_.insert(size, value);
  }

  /// Set the value
  Token& SetValue(const string_type& value)
  {
    value_ = value;
  }

  /// Clear the value
  Token& ClearValue()
  {
    value_.clear();
  }

private:

  string_type value_;                       ///< The value of the token
  TokenType   type_;                        ///< The type for the stored token
};

LEXER_END

#endif  // _token_h_
