#ifndef _tools_h_
#define _tools_h_

#include "common.h"

LEXER_START
TOOLS_START

/// Struct for checking if 2 types are equivalent at compile time
template <class myType1, class myType2>
struct same_type
{
  enum { value = false };
};

/// Specialization for checking if 2 types are equivalent at compile time
template <class myType>
struct same_type<myType, myType>
{
  enum { value = true };
};

/// Struct for returning a certain type if 2 types are equivalent at compile time
template <class my_type_LHS, class my_type_RHS,
          class eq_type, class dif_type>
struct if_is_of_type
{
  typedef dif_type ret_type;
};

/// Specialization for returning a certain type if 2 types are equivalent at compile time
template <class my_type, 
          class eq_type, class dif_type>
struct if_is_of_type<my_type, my_type, 
                     eq_type, dif_type>
{
  typedef eq_type ret_type;
};

template <class mytype>
struct is_character
{
  enum { value = false };
};

template <>
struct is_character<char>
{
  enum { value = true };
};

template <>
struct is_character<wchar_t>
{
  enum { value = true };
};

TOOLS_END
LEXER_END

#endif // _tools_h_