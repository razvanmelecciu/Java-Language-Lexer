#ifndef _token_h_
#define _token_h_

#include "common.h"
#include "tools.hpp"
#include "grammar_symbols.hpp"
#include <string>
#include <set>

LEXER_START

/////////////////////////////////////////////////////////////////////////
/// A class for keeping track of all the string values of the various tokens 
/// issued (to avoid string duplication)
/////////////////////////////////////////////////////////////////////////
template <class char_type = char>
class TokenStringTable
{
  static_assert(lexer::tools::is_character<char_type>::value, "The class template can only be instantiated with char types like char/wchar_t");

  // - Types

public:

  typedef char_type my_char_type;
  typedef typename lexer::tools::if_is_of_type<char_type, wchar_t,
                                               std::wstring, std::string>::ret_type string_type;   ///< compile time type identification
  typedef std::set<string_type> table_type;
  typedef typename table_type::const_iterator const_table_item;

public :

  /// Default ctor
  TokenStringTable()
  {
    // nothing here yet
  }

  /// Copy
  TokenStringTable(const TokenStringTable& RHS) : unique_string_table(RHS.unique_string_table)
  {
  }

#if defined MOVE_SEMANTICS
  /// Move
  TokenStringTable(TokenStringTable&& RHS) : unique_string_table(static_cast<string_type&&>(RHS.unique_string_table))
  {
  }
#endif

  /// Dtor
  ~TokenStringTable()
  {
    // nothing here yet
  }

  // - Accessors

  /// Check if a string is already in the table
  bool HasEntry(const string_type& crt_string) const
  {
    std::set<string_type>::const_iterator my_pos = unique_string_table.find(crt_string);
    if (my_pos != unique_string_table.end())
      return true;
    return false;
  }

  // - Mutators

  /// Add a unique string to the current table (only if it's not there already)
  const_table_item AddString(const string_type& crt_string, bool* added_flag = nullptr)
  {
    std::set<string_type>::_Pairib ins_val = unique_string_table.insert(crt_string);
    if (added_flag)
      *added_flag = ins_val.second;

    return ins_val.first;
  }

private :
 
  table_type unique_string_table;
};

/////////////////////////////////////////////////////////////////////////
/// A token class which keeps its string in the specified TokenStringTable
/////////////////////////////////////////////////////////////////////////
template <class char_type = char>
class Token
{
  static_assert(lexer::tools::is_character<char_type>::value, "The class template can only be instantiated with char types like char/wchar_t");

  // - Types

public:

  typedef char_type my_char_type;
  typedef typename lexer::tools::if_is_of_type<char_type, wchar_t, 
                                               std::wstring, std::string>::ret_type string_type;   ///< compile time type identification
  typedef typename TokenStringTable<char_type>::const_table_item tbl_item_ref;

public :

  /// Ctor
  Token(TokenStringTable<char_type>& string_table, const string_type& value, IdentifierCategory token_type) : type_(token_type)
  {
    table_item_ref_it = string_table.AddString(value);
  }

  /// Copy
  Token(const Token& RHS) : type_(RHS.type_), table_item_ref_it(RHS.table_item_ref_it)
  {
  }  

  // - Accessors

  /// Get the current type
  IdentifierCategory GetType() const
  {
    return type_;
  }

  /// Get the current value
  void GetValue(string_type& value) const
  {
    value.clear();
    value = *table_item_ref_it;
  }

  /// Has value
  bool HasValue() const
  {
    bool has_value = table_item_ref_it->size() > 0;
    return has_value;
  }

private:

  IdentifierCategory   type_;                                                         ///< The type for the stored token
  tbl_item_ref table_item_ref_it;  ///< The value of the string from the table stored as a const iterator
};


LEXER_END

#endif  // _token_h_
