#include <vector>
#include <stack>
#include <string>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

enum TokenType {Initial=0, Number=1, Operator=2, LBracket=4, RBracket=5};

struct Token {
    TokenType type = Initial;
    uint8_t precedence = 0;
    std::string characters = "";
    double value = 0.0;
};

std::vector<Token> tokenize(std::string line) {
    std::vector<Token> tokens;
    uint8_t tokenStart = 0;
    uint8_t tokenLength = 0;
    char chr;
    TokenType prevState, state = Initial;
    for (uint8_t i = 0; i <= line.size(); ++i) {
        chr = i == line.size() ? '\0' : line[i];
        if (chr == ' ') continue; // Ignore spaces
        if ((chr >= '0' && chr <= '9') || chr == '.') {
            state = Number;
        } else if (chr == '+' || chr == '-' || chr == '*' || chr == '/' || chr == '^') {
            state = Operator;
        } else if (chr == '(') {
            state = LBracket;
        } else if (chr == ')') {
            state = RBracket;
        } else if (chr == '\0') {
            state = Initial;
        } else {
            std::cerr << "Unknown symbol " << chr << std::endl;
            continue;
        }
        if (prevState != state && tokenLength) {
            Token token;
            token.type = prevState;
            token.type = Number;
            token.characters = line.substr(tokenStart, tokenLength);
            if (prevState == Operator) {
                if (token.characters == "+" || token.characters == "-") {
                    token.precedence = 0;
                } else if (token.characters == "*" || token.characters == "/") {
                    token.precedence = 1;
                } else if (token.characters == "^") {
                    token.precedence = 2;
                }
            } else if (prevState == Number) {
                token.value = std::stod(token.characters);
            } 
            tokens.push_back(token);
            tokenStart = i;
            tokenLength = 0;
        }
        tokenLength++;
        prevState = state;
    }
    return tokens;
}

std::vector<Token> shuntingYard(std::vector<Token> tokens) {
    std::vector<Token> queue;
    std::stack<Token> stack;
    for (std::vector<Token>::iterator token = tokens.begin(); token != tokens.end(); ++token) {
        if (token->type == Number) {
            queue.push_back(*token);
        } else if (token->type == Operator) {
            while (!stack.empty() && stack.top().type != LBracket && stack.top().precedence > token->precedence) {
                queue.push_back(stack.top());
                stack.pop();
            }
            stack.push(*token);
        } else if (token->type == LBracket) {
            stack.push(*token);
        } else if (token->type == RBracket) {
            while (!stack.empty() && stack.top().type != LBracket) {
                queue.push_back(stack.top());
                stack.pop();
            }
            if (!stack.empty()) {
                stack.pop();
            }
        }
    }
    while (!stack.empty()) {
        queue.push_back(stack.top());
        stack.pop();
    }
    return queue;
}

double calculate(std::vector<Token> tokens) {
    std::stack<Token> stack;
    while (!tokens.empty()) {
        Token token = *(tokens.begin());
        tokens.erase(tokens.begin());
        if (token.type == Number) {
            stack.push(token);
        } else if (token.type == Operator) {
            Token right = stack.top();
            stack.pop();
            Token left = stack.top();
            stack.pop();
            double r = 0.0;
            if (token.characters == "+") {
                r = left.value + right.value;
            } else if (token.characters == "-") {
                r = left.value - right.value;
            } else if (token.characters == "*") {
                r = left.value * right.value;
            } else if (token.characters == "/") {
                r = left.value / right.value;
            } else if (token.characters == "^") {
                r = pow(left.value, right.value);
            }
            Token result;
            result.value = r;
            stack.push(result);
        }
    }
    return stack.top().value;
}

void printToken(Token token) {
    std::cout << "Token <" << token.characters
              << "> (type=" << token.type
              << ", precedence=" << (int)token.precedence
              << ", value=" << (double)token.value
              << ")\n";
}

void printTokens(std::vector<Token> tokens) {
    for (std::vector<Token>::iterator token = tokens.begin(); token != tokens.end(); ++token) {
        printToken(*token);
    }
}

int main(int argc, char *argv[]) {
    int8_t i;
    for (i = 1; i < argc; i++) {
        std::vector<Token> tokens = tokenize(argv[i]);
        printTokens(tokens);
        std::vector<Token> rpn = shuntingYard(tokens);
        double result = calculate(rpn);
        std::cout << result << std::endl;
    }
    return 0;
}