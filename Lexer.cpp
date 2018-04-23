// Lexer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "src/lexer.hpp"
#include "src/alphabet_test.hpp"
#include "src/DFA_test.hpp"

#include <fstream>
#include <iostream>
#include <iomanip>
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
  std::string parsed_file_name("samples//javap2.java");
  std::string output_file_name("samples//lex_javap2.out");

  // - Input file name
  /*std::cout << "Insert the parsed file in the format [path-file-extension]\n";
  std::cin >> parsed_file_name;

  // - Output file name
  std::cout << "Insert the path for the output file [in the format path-file-extension]\n";
  std::cin >> output_file_name;*/

  std::ifstream input_file_stream(parsed_file_name, std::ifstream::in);
  std::ofstream output_file_stream(output_file_name, std::ifstream::out);
  
  // - Check files opened
  if (!input_file_stream.is_open())
  {
    std::cout << "Invalid file names specified!\n";
    return 1;
  }

  // - Instantiate the lexer
  lexer::GrammarIdentifiers<char> grammar_identifiers;
  lexer::Lexer language_scanner(input_file_stream, grammar_identifiers);
  lexer::Lexer::token_list my_list;

  char crt_character = '\0';
  std::string token_value, token_identifier_in_clear;
  lexer::IdentifierCategory token_identifier;

  do
  {
    lexer::DFA<char, unsigned short, unsigned short>::Messages ret_code = language_scanner.GetToken(my_list);

    if (ret_code == lexer::Lexer::Messages::UNRECOGNIZED)
      output_file_stream << "Lexical analysis error - Pos No: " << language_scanner.GetStreamPosition() << std::endl;

    for (const auto& elem : my_list)
    {
      token_identifier = elem.GetType();
      if (token_identifier == lexer::WHITE_SPACE || token_identifier == lexer::INLINE_COMMENT ||
          token_identifier == lexer::MULTI_LINE_COMMENT)                                          // discard the WHITE_SPACE category
        continue;

      elem.GetValue(token_value);

      switch (token_identifier)
      {
      case lexer::KEYWORD :
        token_identifier_in_clear = "KEYWORD";
        break;
      case lexer::IDENTIFIER :
        token_identifier_in_clear = "IDENTIFIER";
        break;
      case lexer::BOOLEAN_LITERAL :
        token_identifier_in_clear = "BOOLEAN LITERAL";
        break;
      case lexer::PREDEFINED_TYPE :
        token_identifier_in_clear = "PREDEFINED TYPE";
        break;
      case lexer::INTEGER_LITERAL :
        token_identifier_in_clear = "INTEGER LITERAL";
        break;
      case lexer::FLOATING_POINT_LITERAL :
        token_identifier_in_clear = "FLOATING POINT LITERAL";
        break;
      case lexer::CHARACTER_LITERAL :
        token_identifier_in_clear = "CHARACTER LITERAL";
        break;
      case lexer::STRING_LITERAL :
        token_identifier_in_clear = "STRING LITERAL";
        break;
      case lexer::OPERATOR :
        token_identifier_in_clear = "OPERATOR";
        break;
      case lexer::WHITE_SPACE :
        token_identifier_in_clear = "WHITE SPACE";
        break;
      case lexer::INLINE_COMMENT :
        token_identifier_in_clear = "INLINE COMMENT";
        break;
      case lexer::MULTI_LINE_COMMENT :
        token_identifier_in_clear = "MULTI LINE COMMENT";
        break;
      case lexer::BRACKET :
        token_identifier_in_clear = "BRACKET";
        break;
      case lexer::SEPARATOR :
        token_identifier_in_clear = "SEPARATOR";
        break;
      default:
        token_identifier_in_clear = "UNKNOWN";
        break;
      }

      output_file_stream << std::left << std::setw(28) << (std::string("Type: ") + token_identifier_in_clear) << " Value: " << token_value << std::endl;
    }
  } while (!input_file_stream.eof());

  // - Close the file streams
  input_file_stream.close();
  output_file_stream.close();

  return 0;
}

