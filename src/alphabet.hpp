#ifndef _alphabet_h
#define _alphabet_h

#include "common.h"
#include <set>

AUTOMATON_START
TOOLS_START

/// Some basic categories for subsets for an alphabet
enum DefaultAlphabetSubset : unsigned short                   
{
  NONE = 0x00, UPR_LETTERS = 0x01, LWR_LETTERS = 0x02, DIGITS = 0x04,
  MATH_OPS = 0x08, PUNCTUATION = 0x10, PARANTHESES = 0x20, ALL = 0xFF
};

////////////////////////////////////////////////////////////////////////////////////////
/// A basic generic alphabet class. The alphabet can contain basic subsets or can keep
/// custom character types, besides the defined subsets. E.g. the alphabet can have only 
/// numbers plus the characters 'A' and 'B'.
////////////////////////////////////////////////////////////////////////////////////////
template <class char_type = char, 
          class mask_type = unsigned short,
          class subset_type = DefaultAlphabetSubset>
class Alphabet
{
  // - Types

public :

  typedef char_type character_type;                        ///< indicates the current character type for this specialization (char, wchar_t)
  typedef mask_type mask_attrib_type;                      ///< indicates the type used for keeping the subsets (integral type)
  typedef subset_type alphabet_subset;                     ///< indicates the type that determines my subsets

public :

  /// Ctor
  explicit Alphabet(mask_attrib_type initial_subset = ALL) : mask_included_subset_(initial_subset)
  {
  }

  /// Copy ctor
  Alphabet(const Alphabet& RHS_Object) : mask_included_subset_(RHS_Object.mask_included_subset_)
  {
  }

#if defined MOVE_SEMANTICS
  /// Move ctor
  Alphabet(Alphabet&& RHS_Orphaned) : mask_included_subset_(RHS_Orphaned.mask_included_subset_),
                                                        additionally_added_chars_(static_cast<std::set<char>&&>(RHS_Orphaned.additionally_added_chars_))
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
      mask_included_subset_ = RHS.mask_included_subset_;
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
    {
      mask_included_subset_ = RHS_Orphaned.mask_included_subset_;
      additionally_added_chars_ = static_cast<std::set<char>&&>(RHS_Orphaned.additionally_added_chars_);
    }

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

  /// Check if the current alphabet includes the specified subset
  bool HasSubset(mask_attrib_type subset) const
  {
    unsigned short result = mask_included_subset_ & mask_included_subset_;
    return result != 0;
  }

  /// Empty alphabet
  bool IsEmpty() const
  {
    if (mask_included_subset_ == NONE && additionally_added_chars_.empty())
      return true;
    return false;
  }

  /// Checks the number of custom values from the alphabet
  std::size_t NoCustomAlphabetCharacters() const
  {
    return additionally_added_chars_.size();
  }

  // - Mutators

  /// Reset alphabet (no characters included by default)
  Alphabet& ResetAlphabet(mask_attrib_type initial_subset = 0x00)
  {
    mask_included_subset_ = initial_subset;
  }

  /// Remove all the added characters (only keeps the subsets)
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

  mask_type                mask_included_subset_;         ///< The mask of included ASCII characters
  std::set<character_type> additionally_added_chars_;     ///< Additionally added characters, besides the subsests added
};

TOOLS_END
AUTOMATON_END

#endif // _alphabet_h
