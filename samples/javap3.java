package test;

import lexer.Lexer;
import lexer.Token;
import lexer.TokenType;
import org.junit.jupiter.api.Test;
import utilities.DummyReader;

class LexerOperatorTokenTest {

    @Test
    void readLessThanOperatorToken() {
        DummyReader dummyReader = new DummyReader("a<b");
        Lexer lexer = new Lexer(dummyReader);
        lexer.getToken();
        Token token = lexer.getToken();

        TestUtils.validateTokenType(token, TokenType.OPERATOR);
    }

    @Test
    void readLessOrEqThanOperatorToken() {
        DummyReader dummyReader = new DummyReader("a<=b");
        Lexer lexer = new Lexer(dummyReader);
        lexer.getToken();
        Token token = lexer.getToken();

        TestUtils.validateTokenType(token, TokenType.OPERATOR);
        TestUtils.validateTokenValue(token, "<=");
    }

    @Test
    void readPlusOperatorToken() {
        DummyReader dummyReader = new DummyReader("+");
        Lexer lexer = new Lexer(dummyReader);
        Token token = lexer.getToken();

        TestUtils.validateTokenType(token, TokenType.OPERATOR);
    }

    @Test
    void readPlusPlusOperatorToken() {
        DummyReader dummyReader = new DummyReader("a++");
        Lexer lexer = new Lexer(dummyReader);
        lexer.getToken();
        Token token = lexer.getToken();

        TestUtils.validateTokenType(token, TokenType.OPERATOR);
        TestUtils.validateTokenValue(token, "++");
    }

    @Test
    void readDivideOperatorToken() {
        DummyReader dummyReader = new DummyReader("10/5;");
        Lexer lexer = new Lexer(dummyReader);
        lexer.getToken();
        Token token = lexer.getToken();

        TestUtils.validateTokenType(token, TokenType.OPERATOR);
        TestUtils.validateTokenPosition(token, 2, 0);

    }

}
