// Lexer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "src/alphabet_test.hpp"
#include "src/DFA_test.hpp"
#include <iostream>
#include <string>
#include "src/token.hpp"

struct TestRunner
{
  static void RunTest(bool(*pFn)(), const std::string& description)
  {
    if (pFn() == true)
      std::cout << description<< " " << "[Test passed]"<< std::endl;
    else
      std::cout << description << " " << "[Test failed]" << std::endl;
  }
};

int main(int argv, char** argc)
{
  namespace ltt = lexer::tools::test;
  namespace ldt = lexer::test;

  TestRunner::RunTest(&ltt::TestAlphabet::RunTest1, "Alphabet-Test1");
  TestRunner::RunTest(&ltt::TestAlphabet::RunTest2, "Alphabet-Test2");

  TestRunner::RunTest(&ldt::TestDFA::RunTest1, "DFA-Test1");
  TestRunner::RunTest(&ldt::TestDFA::RunTest2, "DFA-Test2");

  lexer::TokenStringTable<> strtabl;
  lexer::Token<> t1(strtabl, "val1", lexer::KEYWORD);
  lexer::Token<> t2(strtabl, "val1", lexer::KEYWORD);
  lexer::Token<> t3(strtabl, "val2", lexer::KEYWORD);

  char x;
  std::cin>>x;

  return 0;
}

