// Lexer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "src/alphabet.hpp"
#include "src/DFA.hpp"

int main(int argv, char** argc)
{
  namespace at = automaton::tools;
  namespace ad = automaton::dfa;

  at::Alphabet<char, unsigned short, at::DefaultAlphabetSubset> myAlphabet(at::ALL);
  automaton::dfa::DFA<> myDFA;



  return 0;
}

