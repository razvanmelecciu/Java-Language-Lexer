@echo off

echo Running All the samples
pushd "x64\Debug\"
echo.
echo ### Test 1
Lexer.exe "..\..\samples/javap1.java" "..\..\samples/lex_javap1.out"
echo.
echo ### Test 2
Lexer.exe "..\..\samples/javap2.java" "..\..\samples/lex_javap2.out"
echo.
echo ### Test 3
Lexer.exe "..\..\samples/javap3.java" "..\..\samples/lex_javap3.out"
echo.
echo ### Test 4
Lexer.exe "..\..\samples/javap4.java" "..\..\samples/lex_javap4.out"
echo.
echo ### Test 5
Lexer.exe "..\..\samples/javap5.java" "..\..\samples/lex_javap5.out"
echo.
echo ### Test 6
Lexer.exe "..\..\samples/javap6.java" "..\..\samples/lex_javap6.out"
echo.
echo ### Test 7
Lexer.exe "..\..\samples/javap7.java" "..\..\samples/lex_javap7.out"
echo.
echo ### Test 8
Lexer.exe "..\..\samples/javap8.java" "..\..\samples/lex_javap8.out"
popd
