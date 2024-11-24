#include <iostream>
#include <cstring>
#include "token.h"
#include "scanner.h"

using namespace std;

Scanner::Scanner(const char* s):input(s),first(0), current(0) { }


bool is_white_space(char c) {
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

Token* Scanner::nextToken() {
    Token* token;
    while (current < input.length() &&  is_white_space(input[current]) ) current++;
    if (current >= input.length()) return new Token(Token::END);
    char c  = input[current];
    first = current;



    if (c == '/' && current + 1 < input.length() && input[current + 1] == '/') {
        current += 2;
        while (current < input.length() && input[current] != '\n') {
            current++;
        }
        return nextToken();
    }


    if (isdigit(c)) {
        current++;
        while (current < input.length() && isdigit(input[current]))
            current++;
        token = new Token(Token::NUM, input, first, current - first);
    }

    else if (isalpha(c)) {
        current++;
        while (current < input.length() && isalnum(input[current]))
            current++;
        string word = input.substr(first, current - first);
        if (word == "println") {
            current++;
            token = new Token(Token::PRINTLN, word, 0, word.length());
        } else if (word == "if") {
            token = new Token(Token::IF, word, 0, word.length());
        } else if (word == "else") {
            token = new Token(Token::ELSE, word, 0, word.length());
        } else if (word == "while") {
            token = new Token(Token::WHILE, word, 0, word.length());
        } else if (word == "let") {
            token = new Token(Token::LET, word, 0, word.length());
        } else if (word == "mut") {
            token = new Token(Token::MUT, word, 0, word.length());
        } else if (word == "for") {
            token = new Token(Token::FOR, word, 0, word.length());
        } else if (word == "fn") {
            token = new Token(Token::FN, word, 0, word.length());
        } else if(word == "in") {
            token = new Token(Token::IN, word, 0, word.length());
        }else {
            token = new Token(Token::ID, word, 0, word.length());
        }
    }

    else if (strchr("+-*/()=;,<>{}.:", c) || c == '"') {
        if(c == '"'){
            string word =  "\"{}\"";
            token = new Token(Token::EXPRINT, word, 0 , word.length());
            current += 4;
        } else if(c == '.'){
            string word = "..";
            token = new Token(Token::DDP, word, 0 ,word.length());
            current += 2;
        } else{
        switch(c) {
            case '+': 
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::PLUSEQ, "+=", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::PLUS, c);
                }
                break;
            case '*': token = new Token(Token::MUL, c); break;
            case '/': token = new Token(Token::DIV, c); break;
            case ',': token = new Token(Token::COMA, c); break;
            case '(': token = new Token(Token::PI, c); break;
            case ')': token = new Token(Token::PD, c); break;
            case '{': token = new Token(Token::CI, c); break;
            case '}': token = new Token(Token::CD, c); break;
            case ':': token = new Token(Token::DP, c); break;
            case '-': 
                if (current + 1 < input.length() && input[current + 1] == '>') {
                    token = new Token(Token::Arrow, "->", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::MINUS, c);
                }
            break;
            case '=':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::EQ, "==", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::ASSIGN, c);
                }
                break;
            case '<':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::LE, "<=", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::LT, c);
                }
                break;
            case '>':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::GE, ">=", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::GT, c);
                }
                break;
            case ';': token = new Token(Token::PC, c); break;
            default:
                cout << "No debería llegar acá" << endl;
                token = new Token(Token::ERR, c);
        }
        current++;
        }
    }
    else {
        token = new Token(Token::ERR, c);
        current++;
    }
    return token;
}

void Scanner::reset() {
    first = 0;
    current = 0;
}

Scanner::~Scanner() { }

void test_scanner(Scanner* scanner) {
    Token* current;
    cout << "Iniciando Scanner:" << endl<< endl;
    while ((current = scanner->nextToken())->type != Token::END) {
        if (current->type == Token::ERR) {
            cout << "Error en scanner - carácter inválido: " << current->text << endl;
            break;
        } else {
            cout << *current << endl;
        }
        delete current;
    }
    cout << "TOKEN(END)" << endl;
    delete current;
}