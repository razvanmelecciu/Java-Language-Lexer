#ifndef _alphabet_test_h
#define _alphabet_test_h

#include "alphabet.hpp"

LEXER_START
TOOLS_START
TEST_START

struct TestAlphabet
{
  static bool RunTest1()
  {
    namespace lt = lexer::tools;
    lt::Alphabet<char, lt::DIGITS> myCharAlphabet;
    if (myCharAlphabet.HasCharacterInSubset('1'))
      return true;
    return false;
  }

  static bool RunTest2()
  {
    namespace lt = lexer::tools;
    lt::Alphabet<wchar_t, lt::DIGITS> myUnicodeAlphabet;
    if (myUnicodeAlphabet.HasCharacterInSubset(L'1'))
        return true;
    return false;
  }
};

TOOLS_END
TEST_END
LEXER_END

#endif // _alphabet_test_h

