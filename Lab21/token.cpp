#include <iostream>
#include "token.h"

using namespace std;

Token::Token(Type type):type(type) { text = ""; }

Token::Token(Type type, char c):type(type) { text = string(1, c); }

Token::Token(Type type, const string& source, int first, int last):type(type) {
    text = source.substr(first, last);
}

std::ostream& operator << ( std::ostream& outs, const Token & tok )
{
    switch (tok.type) {
        case Token::PLUS: outs << "TOKEN(PLUS)"; break;
        case Token::MINUS: outs << "TOKEN(MINUS)"; break;
        case Token::MUL: outs << "TOKEN(MUL)"; break;
        case Token::DIV: outs << "TOKEN(DIV)"; break;
        case Token::NUM: outs << "TOKEN(NUM)"; break;
        case Token::ERR: outs << "TOKEN(ERR)"; break;
        case Token::PD: outs << "TOKEN(PD)"; break;
        case Token::PI: outs << "TOKEN(PI)"; break;
        case Token::END: outs << "TOKEN(END)"; break;
        case Token::ID: outs << "TOKEN(ID)"; break;
        case Token::PRINTLN: outs << "TOKEN(PRINTLN)"; break;
        case Token::ASSIGN: outs << "TOKEN(ASSIGN)"; break;
        case Token::PC: outs << "TOKEN(PC)"; break;
        case Token::LT: outs << "TOKEN(LT)"; break;
        case Token::LE: outs << "TOKEN(LE)"; break;
        case Token::EQ: outs << "TOKEN(EQ)"; break;
        case Token::IF: outs << "TOKEN(IF)"; break;
        case Token::ELSE: outs << "TOKEN(ELSE)"; break;
        case Token::WHILE: outs << "TOKEN(WHILE)"; break;
        case Token::COMA: outs << "TOKEN(COMA)"; break;
        case Token::FOR : outs << "TOKEN(FOR)"; break;
        case Token::CD : outs << "TOKEN(CD)"; break;
        case Token::CI : outs << "TOKEN(CI)"; break;
        case Token::LET : outs << "TOKEN(LET)"; break;
        case Token::MUT : outs << "TOKEN(MUT)"; break;
        case Token::FN : outs << "TOKEN(FN)"; break;
        case Token::Arrow : outs << "TOKEN(Arrow)"; break;
        case Token::DP : outs << "TOKEN(DP)";break;
        case Token::DDP : outs << "TOKEN(DDP)"; break;
        case Token::GE : outs << "TOKEN(GE)";break;
        case Token::GT : outs << "TOKEN(GT)";break;
        case Token::PLUSEQ : outs << "TOKEN(PLUSEQ)"; break;
        case Token::EXPRINT : outs << "TOKEN(EXPRINT)"; break;
        case Token::IN : outs << "TOKEN(IN)"; break;
        default: outs << "TOKEN(UNKNOWN)"; break;
    }
    return outs;
}

std::ostream& operator << ( std::ostream& outs, const Token* tok ) {
    return outs << *tok;
}