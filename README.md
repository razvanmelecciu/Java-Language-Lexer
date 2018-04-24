# Java DFA Lexer
A simple educational example of a lexer (scanner) written in C++ that scans Java source files. The implementation is basically based on an underlying DFA object.
The transition function (the DFA rules along with the states) is progressively added to the DFA. (a bit space inefficient but it's quite good to get a grasp on some of the concepts).
Should in theory be able to scan most of the tokens encountered in a Java source file.
Generics are excluded.
![FSM](fsm_schema.jpeg)
