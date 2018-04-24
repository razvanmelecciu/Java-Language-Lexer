// Lexer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "src/lexer.h"
#include "src/alphabet_test.hpp"
#include "src/DFA_test.hpp"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>
#include <array>

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


int main(int argc, char** argv)
{
  namespace lex = lexer;

  std::size_t no_expected_params = 3;
  if (argc != no_expected_params)
  {
    std::cout << "Invalid parameters. Usage is lexer \"path_to_inputfile\" \"path_to_outputfile\"" << std::endl;
    return 1;
  }

  // - File names from parameters
  std::string parsed_file_name(argv[1]);
  std::string output_file_name(argv[2]);

  std::ifstream input_file_stream(parsed_file_name, std::ifstream::in);
  std::ofstream output_file_stream(output_file_name, std::ifstream::out);
  
  // - Check files opened
  if (!input_file_stream.is_open() || !output_file_stream.is_open())
  {
    std::cout << "Error: Invalid file names specified!\n";
    return 1;
  }

  // - Instantiate the lexer
  lex::GrammarIdentifiers<char> grammar_identifiers;
  std::unique_ptr<lex::Lexer> language_scanner(new lex::Lexer(input_file_stream, grammar_identifiers));

  char crt_character = '\0';
  std::size_t output_padding = 28;
  std::string token_value, token_identifier_in_clear;
  lex::IdentifierCategory token_identifier;
  auto start_analysis_time = std::chrono::system_clock::now();
  std::time_t time_stamp = std::chrono::system_clock::to_time_t(start_analysis_time);

  output_file_stream << "Lexical atoms list\n";
  output_file_stream << "File analysed: " << parsed_file_name << std::endl;
  output_file_stream << "Time: " << std::ctime(&time_stamp) <<std::endl;

  std::cout << "File analysed: " << parsed_file_name << std::endl;
  std::cout << "Output file: " << output_file_name << std::endl;
  std::cout << "Decomposing the file into lexical atoms...\n";

  do
  {
    lex::Token<char> ret_token = language_scanner->GetToken();
    token_identifier = ret_token.GetType();

    if (token_identifier == lex::UNRECOGNIZED_ERROR)
    {
      std::cout << "Lexical analysis error - Pos No: \n";
      output_file_stream << "Lexical analysis error - Pos No: " << language_scanner->GetStreamPosition() << std::endl;
    }
    else
    {
      if (token_identifier == lex::WHITE_SPACE || token_identifier == lex::INLINE_COMMENT ||
          token_identifier == lex::MULTI_LINE_COMMENT)                                      // discard the WHITE_SPACE / INLINE_COMMENT / MULTI_LINE_COMMENT category
        continue;

      ret_token.GetValue(token_value);

      switch (token_identifier)
      {
      case lex::KEYWORD:
        token_identifier_in_clear = "KEYWORD";
        break;
      case lex::IDENTIFIER:
        token_identifier_in_clear = "IDENTIFIER";
        break;
      case lex::BOOLEAN_LITERAL:
        token_identifier_in_clear = "BOOLEAN LITERAL";
        break;
      case lex::PREDEFINED_TYPE:
        token_identifier_in_clear = "PREDEFINED TYPE";
        break;
      case lex::INTEGER_LITERAL:
        token_identifier_in_clear = "INTEGER LITERAL";
        break;
      case lex::FLOATING_POINT_LITERAL:
        token_identifier_in_clear = "FLOATING POINT LITERAL";
        break;
      case lex::CHARACTER_LITERAL:
        token_identifier_in_clear = "CHARACTER LITERAL";
        break;
      case lex::STRING_LITERAL:
        token_identifier_in_clear = "STRING LITERAL";
        break;
      case lex::OPERATOR:
        token_identifier_in_clear = "OPERATOR";
        break;
      case lex::WHITE_SPACE:
        token_identifier_in_clear = "WHITE SPACE";
        break;
      case lex::INLINE_COMMENT:
        token_identifier_in_clear = "INLINE COMMENT";
        break;
      case lex::MULTI_LINE_COMMENT:
        token_identifier_in_clear = "MULTI LINE COMMENT";
        break;
      case lex::BRACKET:
        token_identifier_in_clear = "BRACKET";
        break;
      case lex::SEPARATOR:
        token_identifier_in_clear = "SEPARATOR";
        break;
      default:
        token_identifier_in_clear = "UNKNOWN";
        break;
      }

      output_file_stream << std::left << std::setw(output_padding) << (std::string("Type: ") + token_identifier_in_clear) << " Value: " << token_value << std::endl;
    }
  } while (!input_file_stream.eof());

  std::cout << "Analysis finished.Check the end result.\n";

  // - Close the file streams
  input_file_stream.close();
  output_file_stream.close();

  return 0;
}

