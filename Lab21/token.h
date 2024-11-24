#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token {
public:
    enum Type {
        PLUS, 
        MINUS, 
        MUL, 
        DIV, 
        NUM, 
        ERR, 
        PD, 
        PI, 
        END, 
        ID, 
        PRINTLN, // println!
        EXPRINT, // "{}"
        ASSIGN, 
        GE,
        GT,
        PC,
        LT,
        LE, 
        EQ, 
        IF, 
        ELSE,
        CD,     // {
        CI,     // }
        WHILE, 
        COMA,
        LET,    // let
        MUT,    // mut
        FOR,
        FN,     // fn
        Arrow, // ->
        DP,  // :
        DDP, // ..
        PLUSEQ, // += 
        IN
    };

    Type type;
    std::string text;

    Token(Type type);
    Token(Type type, char c);
    Token(Type type, const std::string& source, int first, int last);

    friend std::ostream& operator<<(std::ostream& outs, const Token& tok);
    friend std::ostream& operator<<(std::ostream& outs, const Token* tok);
};

#endif // TOKEN_H