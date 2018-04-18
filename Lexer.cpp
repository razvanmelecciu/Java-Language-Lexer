// Lexer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "src/alphabet.hpp"
#include "src/DFA.hpp"
#include <iostream>

int main(int argv, char** argc)
{
  namespace at = automaton::tools;
  namespace ad = automaton::dfa;

  at::Alphabet<char, unsigned short, at::DefaultAlphabetSubset> myAlphabet(at::ALL);
  ad::DFA<> myDFA;

  myDFA.SetInitialState(0);
  myDFA.AddFinalState(0);
  myDFA.AddTransition(0, '0', 0);
  myDFA.AddTransition(0, '1', 1);
  myDFA.AddTransition(1, '1', 0);
  myDFA.AddTransition(1, '0', 2);
  myDFA.AddTransition(2, '0', 1);
  myDFA.AddTransition(2, '1', 2);

  ad::DFA<>::Messages ret = myDFA.Accepts("01", 2);
  
  if (ret == ad::DFA<>::Messages::RECOGNIZED)
    std::cout << "Accepted";
  else
    std::cout << "Not accepted";

  char x;
  std::cin>>x;

  return 0;
}

