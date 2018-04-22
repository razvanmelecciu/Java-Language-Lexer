#ifndef _DFA_test_h
#define _DFA_test_h

#include "DFA.hpp"

LEXER_START
TEST_START

struct TestDFA
{
  static bool RunTest1()
  {
    namespace ad = lexer;

    ad::DFA<> myDFA;
    myDFA.SetInitialState(0);
    myDFA.AddFinalState(0, 0);
    myDFA.AddTransition(0, '0', 0);
    myDFA.AddTransition(0, '1', 1);
    myDFA.AddTransition(1, '1', 0);
    myDFA.AddTransition(1, '0', 2);
    myDFA.AddTransition(2, '0', 1);
    myDFA.AddTransition(2, '1', 2);

    ad::DFA<>::Messages ret = myDFA.Accepts("01", 2);

    if (ret == ad::DFA<>::Messages::RECOGNIZED)
      return false;
    else
      return true;
  }

  static bool RunTest2()
  {
    namespace ad = lexer;

    ad::DFA<wchar_t> myDFA;
    myDFA.SetInitialState(0);
    myDFA.AddFinalState(0, 0);
    myDFA.AddTransition(0, L'0', 0);
    myDFA.AddTransition(0, L'1', 1);
    myDFA.AddTransition(1, L'1', 0);
    myDFA.AddTransition(1, L'0', 2);
    myDFA.AddTransition(2, L'0', 1);
    myDFA.AddTransition(2, L'1', 2);

    ad::DFA<wchar_t>::Messages ret = myDFA.Accepts(L"01", 2);

    if (ret == ad::DFA<wchar_t>::Messages::RECOGNIZED)
      return false;
    else
      return true;
  }
};

TEST_END
LEXER_END

#endif // _DFA_test_h