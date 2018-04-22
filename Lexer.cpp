// Lexer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "src/lexer.hpp"
#include "src/alphabet_test.hpp"
#include "src/DFA_test.hpp"

#include <fstream>
#include <iostream>
#include <string>

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

void RunAllTests()
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
}

int main(int argv, char** argc)
{
  std::string parsed_file_name;
  std::string output_file_name;

  // - Input file name
  std::cout << "Insert the parsed file in the format [path-file-extension]\n";
  std::cin >> parsed_file_name;

  // - Output file name
  std::cout << "Insert the path for the output file [in the format path-file-extension]\n";
  std::cin >> output_file_name;

  std::ifstream input_file_stream(parsed_file_name, std::ifstream::in);
  std::ofstream output_file_stream(output_file_name, std::ifstream::out);
  
  // - Check files opened
  if (!input_file_stream.is_open() || !output_file_stream.is_open())
  {
    std::cout << "Invalid file names specified!\n";
    return 1;
  }

  // - Instantiate the lexer
  lexer::GrammarIdentifiers<char> grammar_identifiers;
  lexer::Lexer language_scanner(input_file_stream, grammar_identifiers);



  // - Close the file streams
  input_file_stream.close();
  output_file_stream.close();

  return 0;
}

