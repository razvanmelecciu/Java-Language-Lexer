#ifndef _alphabet_h
#define _alphabet_h

#include "common.h"
#include "tools.hpp"
#include <set>

LEXER_START
TOOLS_START

/// Some basic categories for subsets for an alphabet
enum DefaultAlphabetSubset : unsigned short                   
{
  NONE = 0x00, UPR_LETTERS = 0x01, LWR_LETTERS = 0x02, DIGITS = 0x04,
  MATH_OPS = 0x08, PUNCTUATION = 0x10, BRACKETS = 0x20, ALL = 0xFF
};

/// Templated class for keeping different subsets of an alphabet
template <class char_type, DefaultAlphabetSubset crt_subset = NONE>
struct SubsetCheckType
{
  enum { subset = crt_subset };

  static bool CheckSubset(char_type* sequence, std::size_t sequence_size)
  {
    return false;
  }
};

#pragma region SubsetCheckTypeSpecializations

// - Partial specializations for char and wchar_t

template <>
struct SubsetCheckType<char, ALL>
{
  enum { subset = ALL };

  static bool CheckSubset(const char* sequence, std::size_t sequence_size)
  {
    return true;
  }
};

template <>
struct SubsetCheckType<wchar_t, ALL>
{
  enum { subset = ALL };

  static bool CheckSubset(const wchar_t* sequence, std::size_t sequence_size)
  {
    return true;
  }
};

template <>
struct SubsetCheckType<char, UPR_LETTERS>
{
  enum { subset = UPR_LETTERS };

  static bool CheckSubset(const char* sequence, std::size_t sequence_size)
  {
    for (std::size_t i = 0; i < sequence_size; ++i)
    {
      if (sequence[i] < 'A' && sequence[i] > 'Z')
        return false;
    }

    return true;
  }
};

template <>
struct SubsetCheckType<wchar_t, UPR_LETTERS>
{
  enum { subset = UPR_LETTERS };

  static bool CheckSubset(const wchar_t* sequence, std::size_t sequence_size)
  {
    for (std::size_t i = 0; i < sequence_size; ++i)
    {
      if (sequence[i] < L'A' && sequence[i] > L'Z')
        return false;
    }

    return true;
  }
};

template <>
struct SubsetCheckType<char, LWR_LETTERS>
{
  enum { subset = LWR_LETTERS };

  static bool CheckSubset(const char* sequence, std::size_t sequence_size)
  {
    for (std::size_t i = 0; i < sequence_size; ++i)
    {
      if (sequence[i] < 'a' && sequence[i] > 'z')
        return false;
    }

    return true;
  }
};

template <>
struct SubsetCheckType<wchar_t, LWR_LETTERS>
{
  enum { subset = LWR_LETTERS };

  static bool CheckSubset(const wchar_t* sequence, std::size_t sequence_size)
  {
    for (std::size_t i = 0; i < sequence_size; ++i)
    {
      if (sequence[i] < L'a' && sequence[i] > L'z')
        return false;
    }

    return true;
  }
};

template <>
struct SubsetCheckType<char, DIGITS>
{
  enum { subset = DIGITS };

  static bool CheckSubset(const char* sequence, std::size_t sequence_size)
  {
    for (std::size_t i = 0; i < sequence_size; ++i)
    {
      if (sequence[i] < '0' && sequence[i] > '9')
        return false;
    }

    return true;
  }
};

template <>
struct SubsetCheckType<wchar_t, DIGITS>
{
  enum { subset = DIGITS };

  static bool CheckSubset(const wchar_t* sequence, std::size_t sequence_size)
  {
    for (std::size_t i = 0; i < sequence_size; ++i)
    {
      if (sequence[i] < L'0' && sequence[i] > L'9')
        return false;
    }

    return true;
  }
};

template <>
struct SubsetCheckType<char, MATH_OPS>
{
  enum { subset = MATH_OPS };

  static bool CheckSubset(const char* sequence, std::size_t sequence_size)
  {
    for (std::size_t i = 0; i < sequence_size; ++i)
    {
      switch (sequence[i])
      {
      case '%' :
      case '(' :
      case ')' :
      case '*' :
      case '+' :
      case '-' :
      case '/' :
      case '<' :
      case '>' :
      case '=' :
        // nothing here...keep rolling
        break;
      default :
        return false;
      }
    }

    return true;
  }
};

template <>
struct SubsetCheckType<wchar_t, MATH_OPS>
{
  enum { subset = MATH_OPS };

  static bool CheckSubset(const wchar_t* sequence, std::size_t sequence_size)
  {
    for (std::size_t i = 0; i < sequence_size; ++i)
    {
      switch (sequence[i])
      {
      case L'%':
      case L'(':
      case L')':
      case L'*':
      case L'+':
      case L'-':
      case L'/':
      case L'<':
      case L'>':
      case L'=':
        // nothing here...keep rolling
        break;
      default:
        return false;
      }
    }

    return true;
  }
};

template <>
struct SubsetCheckType<char, BRACKETS>
{
  enum { subset = BRACKETS };

  static bool CheckSubset(const char* sequence, std::size_t sequence_size)
  {
    for (std::size_t i = 0; i < sequence_size; ++i)
    {
      switch (sequence[i])
      {
      case '[':
      case ']':
      case '{':
      case '}':
        // nothing here...keep rolling
        break;
      default:
        return false;
      }
    }

    return true;
  }
};

template <>
struct SubsetCheckType<wchar_t, BRACKETS>
{
  enum { subset = BRACKETS };

  static bool CheckSubset(const wchar_t* sequence, std::size_t sequence_size)
  {
    for (std::size_t i = 0; i < sequence_size; ++i)
    {
      switch (sequence[i])
      {
      case L'[':
      case L']':
      case L'{':
      case L'}':
        // nothing here...keep rolling
        break;
      default:
        return false;
      }
    }

    return true;
  }
};

#pragma endregion

////////////////////////////////////////////////////////////////////////////////////////
/// A basic generic alphabet class. The alphabet can contain basic subsets or can keep
/// custom character types, besides the defined subsets. E.g. the alphabet can have only 
/// numbers plus the characters 'A' and 'B'.
////////////////////////////////////////////////////////////////////////////////////////
template <class char_type = char,
          DefaultAlphabetSubset subset = NONE,
          class subset_chk_type = SubsetCheckType<char_type, subset>>
class Alphabet
{
  static_assert(lexer::tools::is_character<char_type>::value, "The class template can only be instantiated with char types like char/wchar_t");

  // - Types

public :

  typedef char_type character_type;                                 ///< indicates the current character type for this specialization (char, wchar_t)
  typedef subset_chk_type alphabet_subset_chk;                      ///< the base subset
  enum { alphabet_subset = subset };                                ///< indicates the type that determines my subsets

public :

  /// Ctor
  Alphabet()
  {
  }

  /// Copy ctor
  Alphabet(const Alphabet& RHS_Object) : additionally_added_chars_(RHS_Object.additionally_added_chars_)
  {
  }

#if defined MOVE_SEMANTICS
  /// Move ctor
  Alphabet(Alphabet&& RHS_Orphaned) : additionally_added_chars_(static_cast<std::set<char>&&>(RHS_Orphaned.additionally_added_chars_))
  {
  }
#endif

  /// Dtor
  ~Alphabet()
  {
    // no manual stuff here
  }

  // - Operators

  /// Copy assignment
  Alphabet& operator = (const Alphabet& RHS)
  {
    if (this != &RHS)
    {
      additionally_added_chars_.clear();
      additionally_added_chars_.insert(RHS.additionally_added_chars_.begin(), RHS.additionally_added_chars_.end());
    }

    return *this;
  }

#if defined MOVE_SEMANTICS
  /// Move assignment
  Alphabet& operator = (Alphabet&& RHS_Orphaned)
  {
    if (this != &RHS_Orphaned)
      additionally_added_chars_ = static_cast<std::set<char>&&>(RHS_Orphaned.additionally_added_chars_);

    return *this;
  }
#endif

  // - Accessors

  /// Check if a specific char is inside this alphabet
  bool HasAdditionalCharacter(character_type current_character) const
  {
    std::set<character_type>::const_iterator found_character = additionally_added_chars_.find(current_character);
    if (found_character != additionally_added_chars_.end())
      return true;
  }

  /// Check if a specific char is inside this alphabet
  bool HasCharacterInSubset(character_type current_character) const
  {
    return alphabet_subset_chk::CheckSubset(&current_character, 1);
  }

  /// Empty alphabet
  bool IsEmpty() const
  {
    if (additionally_added_chars_.size() == 0 &&  == NONE)
      return true;
    return false;
  }

  /// Checks the number of custom values from the alphabet
  std::size_t NoCustomAlphabetCharacters() const
  {
    return additionally_added_chars_.size();
  }

  // - Mutators

  /// Remove all the added characters
  Alphabet& RemoveAllAdditional()
  {
    additionally_added_chars_.clear();
    return *this;
  }

  /// Add a supplementary character
  bool AddCharacter(character_type added_char)
  {
    std::set<char>::_Pairib ret = additionally_added_chars_.insert(added_char);
    return ret.second;
  }

  // - Members
private :

  std::set<character_type> additionally_added_chars_;     ///< Additionally added characters, besides the subsests added
};

TOOLS_END
LEXER_END

#endif // _alphabet_h
