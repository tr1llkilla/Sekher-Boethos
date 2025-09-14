Copyright © 2025 Cadell Richard Anderson

//Tokenizer.h

#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <cctype>

enum class TokenType {
    EVB_KEYWORD, IF_KEYWORD, ELSE_KEYWORD, WHILE_KEYWORD,
    IDENTIFIER, NUMBER,
    EQUAL, SEMICOLON, L_PAREN, R_PAREN, L_BRACE, R_BRACE, L_BRACKET, R_BRACKET,
    LOGICAL_NOT, LOGICAL_AND, LOGICAL_OR, LOGICAL_XOR,
    PLUS, MINUS, MULTIPLY, DIVIDE,
    GREATER, LESS,
    EQUAL_EQUAL, NOT_EQUAL, GREATER_EQUAL, LESS_EQUAL,
    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string lexeme;
};

class Tokenizer {
public:
    Tokenizer(const std::string& source) : source_(source), current_pos_(0) {}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        while (current_pos_ < source_.length()) {
            if (isspace(source_[current_pos_])) {
                current_pos_++;
                continue;
            }

            if (isalpha(source_[current_pos_])) {
                std::string lexeme = readIdentifierOrKeyword();
                if (lexeme == "evb") { tokens.push_back(createToken(TokenType::EVB_KEYWORD, lexeme)); }
                else if (lexeme == "if") { tokens.push_back(createToken(TokenType::IF_KEYWORD, lexeme)); }
                else if (lexeme == "else") { tokens.push_back(createToken(TokenType::ELSE_KEYWORD, lexeme)); }
                else if (lexeme == "while") { tokens.push_back(createToken(TokenType::WHILE_KEYWORD, lexeme)); }
                else { tokens.push_back(createToken(TokenType::IDENTIFIER, lexeme)); }
            }
            else if (isdigit(source_[current_pos_])) {
                std::string lexeme = readNumber();
                tokens.push_back(createToken(TokenType::NUMBER, lexeme));
            }
            else {
                tokens.push_back(readSymbol());
            }
        }

        tokens.push_back(createToken(TokenType::END_OF_FILE, ""));
        return tokens;
    }

private:
    std::string source_;
    size_t current_pos_;

    Token createToken(TokenType type, const std::string& lexeme) const {
        return { type, lexeme };
    }

    std::string readIdentifierOrKeyword() {
        std::string lexeme = "";
        while (current_pos_ < source_.length() && isalnum(source_[current_pos_])) {
            lexeme += source_[current_pos_++];
        }
        return lexeme;
    }

    std::string readNumber() {
        std::string lexeme = "";
        while (current_pos_ < source_.length() && isdigit(source_[current_pos_])) {
            lexeme += source_[current_pos_++];
        }
        return lexeme;
    }

    Token readSymbol() {
        char c = source_[current_pos_];
        switch (c) {
        case '=':
            if (current_pos_ + 1 < source_.length() && source_[current_pos_ + 1] == '=') { current_pos_ += 2; return createToken(TokenType::EQUAL_EQUAL, "=="); }
            else { current_pos_++; return createToken(TokenType::EQUAL, "="); }
        case ';': current_pos_++; return createToken(TokenType::SEMICOLON, ";");
        case '(': current_pos_++; return createToken(TokenType::L_PAREN, "(");
        case ')': current_pos_++; return createToken(TokenType::R_PAREN, ")");
        case '{': current_pos_++; return createToken(TokenType::L_BRACE, "{");
        case '}': current_pos_++; return createToken(TokenType::R_BRACE, "}");
        case '!': current_pos_++; return createToken(TokenType::LOGICAL_NOT, "!");
        default:
            throw std::runtime_error("Unknown symbol.");
        }
    }
};
