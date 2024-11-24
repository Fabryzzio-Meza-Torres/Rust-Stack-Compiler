#include <iostream>
#include <stdexcept>
#include "token.h"
#include "scanner.h"
#include "exp.h"
#include "parser.h"

using namespace std;

bool Parser::match(Token::Type ttype) {
    if (check(ttype)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(Token::Type ttype) {
    if (isAtEnd()) return false;
    return current->type == ttype;
}

bool Parser::advance() {
    if (!isAtEnd()) {
        Token* temp = current;
        if (previous) delete previous;
        current = scanner->nextToken();
        previous = temp;
        if (check(Token::ERR)) {
            cout << "Error de análisis, carácter no reconocido: " << current->text << endl;
            exit(1);
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd() {
    return (current->type == Token::END);
}

Parser::Parser(Scanner* sc):scanner(sc) {
    previous = NULL;
    current = scanner->nextToken();
    if (current->type == Token::ERR) {
        cout << "Error en el primer token: " << current->text << endl;
        exit(1);
    }
}

VarDec* Parser::parseVarDec() {
    VarDec* vd = NULL;
    if (match(Token::LET)) {
        cout << "let ";
        if(!match(Token::MUT)){
            cout << "Error: se esperaba un 'mut' después de 'let'." << endl;
            exit(1);
        }
        cout << "mut ";
        if (!match(Token::ID)) {
            cout << "Error: se esperaba un identificador después de 'mut'." << endl;
            exit(1);
        }
        cout << previous->text <<  " ";
        list<string> ids;
        ids.push_back(previous->text);
        if (!match(Token::DP)) {
            cout << "Error: se esperaba un ':' despues de un identificador" << endl;
            exit(1);
        }
        cout << ":" << " ";
        if (!match(Token::ID)) {
            cout << "Error: se esperaba un identificador después de ':'." << endl;
            exit(1);
        }
        cout << previous->text;
        string type = previous->text;
        if (!match(Token::PC)) {
            cout << "Error: se esperaba un ';' al final de la declaración." << endl;
            exit(1);
        }
        cout << previous->text << endl;
        vd = new VarDec(type, ids);
        //si con esto no sale no sale con nada zzz
    } else if(previous != NULL && previous->text == "for"){
        match(Token::ID);
        cout << "LLEGA" << endl;
        list<string> ids;
        ids.push_back(previous->text);
        vd = new VarDec("i32", ids);
    }
    
    //acaba aqui
    return vd;
}

VarDecList* Parser::parseVarDecList() {
    VarDecList* vdl = new VarDecList();
    VarDec* aux;
    aux = parseVarDec();
    while (aux != NULL) {
        vdl->add(aux);
        aux = parseVarDec();
    }
    return vdl;
}

StatementList* Parser::parseStatementList() {
    StatementList* sl = new StatementList();
    sl->add(parseStatement());
    Stm* curr ;
    while (true) {
        if(!check(Token::CD)){
            curr = parseStatement();
            sl->add(curr);
        }else{
            match(Token::PC);
            break;
        }
    }
    return sl;
}


Body* Parser::parseBody() {
    VarDecList* vdl = parseVarDecList();
    StatementList* sl = parseStatementList();
    return new Body(vdl, sl);
}

FunDec* Parser::parseFunDec() {
  FunDec* fd = NULL;
  if (match(Token::FN)) {
    cout << "FN: " << endl;
    cout << previous ->text << endl << endl;
    Body* body = NULL;
    Exp* CExp  = NULL;
    if (!match(Token::ID)) {
        cout << "Error: se esperaba un tipo 'fun'." << endl;
        exit(1);
    }
    cout << "ID: " << endl;
    cout << previous ->text << endl << endl;
    string fname = previous->text;
    list<string> types;
    list<string> vars;
    if(!match(Token::PI)){
        cout << "Error: se esperaba un parentesis izquierdo" << endl;   
    }
    cout << "PI: " << endl;
    cout << previous ->text << endl << endl;
    if (!check(Token::PD)) {
        if (!match(Token::ID)) {
            cout << "Error: se esperaba un tipo después del '('." << endl;
            exit(1);
        }
        cout << "ID: " << endl;
        cout << previous ->text << endl << endl;
        vars.push_back(previous->text);
        if(!match(Token::DP)){
            cout << "Falta los dos puntos" << endl;
            exit(1);
        }
        cout << "DP: " << endl;
        cout << previous ->text << endl << endl;
        if (!match(Token::ID)) {
            cout << "Error: se esperaba un tipo después del identificador." << endl;
            exit(1);
        }
        cout << "ID: " << endl;
        cout << previous ->text << endl << endl;
        types.push_back(previous->text);

        while(match(Token::COMA)) {
            cout << "FN: " << endl;
            cout << previous ->text << endl << endl;
            if(!match(Token::ID)){
                cout<< "Error: faltan un id" << endl;
                exit(1);
            }
            vars.push_back(previous->text);
            if(!match(Token::DP)){
                cout << "Error: faltan dos puntos"<<endl;
                exit(1);
            }
            if(!match(Token::ID)) {
            cout << "Error: se esperaba un tipo después de ','." << endl;
            exit(1);
            }
            types.push_back(previous->text);
      }
    }
    if (!match(Token::PD)) {
        cout << "Error: se esperaba un ')' después de la lista de argumentos." << endl;
        exit(1);
    }
    cout << "FN: " << endl;
    cout << previous ->text << endl << endl;
    string rtype; 
    bool isVoid = 1;
    if(match(Token::Arrow)){
        cout << previous->text << endl << endl;
        if(!match(Token::ID)){
            cout << "Error: No tiene tipo la funcion"  << endl;
            exit(1);
        }
        isVoid = 0;
        rtype = previous -> text;
        cout << rtype << endl<<endl;
    } else {
        rtype = "void";
    }
    if(!match(Token::CI)){
        cout << "Error: Falta abrir corchetes de la funcion"  << endl;
        exit(1);
    }
    cout << "CI FunDec: " << endl;
    cout << previous ->text << endl << endl;
    if(!isVoid){
        CExp = parseCExp();
    }else{
        cout << "EMPIEZA BODY FUNDEC:"<< endl;
        body = parseBody();
        cout << "ACABA BODY FUNDEC:"<< endl;
    }
    if(!match(Token::CD)){
        cout << "Error: Falta cerrar corchetes de la funcion"  << endl;
        exit(1);
    }
    cout << "CD FunDec: " << endl;
    cout << previous ->text << endl << endl;
    fd = new FunDec(fname, types, vars, rtype, body, CExp);
  }
  return fd;
}

FunDecList* Parser::parseFunDecList() {
    FunDecList* fdl = new FunDecList();
    FunDec* aux;
    aux = parseFunDec();
    while (aux != NULL) {
        fdl->add(aux);
        aux = parseFunDec();
    }
    return fdl;
}

Program* Parser::parseProgram() {
    VarDecList* v = parseVarDecList();
    FunDecList* b = parseFunDecList();
    return new Program(v, b);
}


Stm* Parser::parseStatement() {
    Stm* s = NULL;
    Exp* e = NULL;
    Body* tb = NULL; //true case
    Body* fb = NULL; //false case
        // cout << "TEST: " << endl;
        // cout << previous -> text << endl;
    if (current == NULL) {
        cout << "Error: Token actual es NULL" << endl;
        exit(1);
    }
    if (check(Token::NUM) || check(Token::PI)){
        e = parseCExp();
    }
    if (match(Token::ID)) {
        string lex = previous->text;
        cout << "ID: " << endl;
        cout << lex << endl << endl;
        if (match(Token::ASSIGN)) {
            cout << "assign: " << endl;
            cout << previous->text << endl << endl;
            e = parseCExp();
            match(Token::PC);
            cout << previous ->text << endl;
            s = new AssignStatement(lex, e , 0);

        }else if(match(Token::PLUSEQ)){
            cout << previous -> text << endl;
            e = parseCExp();
            match(Token::PC);
            s = new AssignStatement(lex, e , 1);
        }else{
            if(match(Token::PI)){
                list<Exp*> args;
                if (!check(Token::PD)){
                    args.push_back(parseCExp());
                    while (match(Token::COMA)){
                        args.push_back(parseCExp());
                    }
                }
                if (!match(Token::PD)){
                    cout << "Falta paréntesis derecho" << endl;
                    exit(0);
                }
                s = new FCallStatement(lex, args);
            }
        }

    } else if (match(Token::PRINTLN)) {
        cout << previous->text << " ";
        if (!match(Token::PI)) {
            cout << "Error: se esperaba un '(' después de 'print'." << endl;
            exit(1);
        }
        cout << previous ->text << " ";
        match(Token::EXPRINT);
        match(Token::COMA);
        e = parseCExp();
        if (!match(Token::PD)) {
            cout << "Error: se esperaba un ')' después de la expresión." << endl;
            exit(1);
        }
        cout << previous->text << endl << endl;
        if(!match(Token::PC)){
            cout << "Error: se esperaba un punto y como en PRINTLN" << endl;
        }
        cout << previous-> text << endl << endl;
        s = new PrintStatement(e);
    }
    else if (match(Token::IF)) {
        e = parseCExp();
        match(Token::CI);
        tb = parseBody();
        match(Token::CD);
        if (match(Token::ELSE)) {
            match(Token::CI);
            fb = parseBody();
            match(Token::CD);
        }
        s = new IfStatement(e, tb, fb);

    }
    else if (match(Token::WHILE)) {
        e = parseCExp();
        if (!match(Token::CI)) {
            cout << "Error: se esperaba un '{' después de la expresión." << endl;
            exit(1);
        }
        cout << previous->text << endl<<endl;
        tb = parseBody();

        if(!match(Token::CD)){
            cout << "Error: se esperaba '}' después de la expresión." << endl;
            exit(1);
        }

        s = new WhileStatement(e, tb);

    }
    else if(match(Token::FOR)){
        VarDecList* vdl = parseVarDecList();
        cout << previous->text << " ";
        string name = previous->text;
        if(!match(Token::IN)){
            cout << "Error: se esperaba un 'in' después de CExp." << endl;
            exit(1);
        }
        cout << previous->text << " ";

        Exp* start = parseCExp();
        if (!match(Token::DDP)) {
            cout << "Error: se esperaba '..' después de la expresión 'in'." << endl;
            exit(1);
        }
        cout << previous->text << " ";
        Exp* end = parseCExp();
        if (!match(Token::CI)) {
            cout << "Error: se esperaba un '{' despues del CExp For." << endl;
            exit(1);
        }
        cout << previous->text << endl<<endl;
        cout << "EMPIEZA BODY FOR:" << endl;
        tb = parseBody();
        cout << "ACABA BODY FOR:" << endl;

        if (!match(Token::CD)) {
            cout << "Error: se esperaba '}' después de la expresión." << endl;
            exit(1);
        }
        cout << previous->text << endl<<endl ;
        Exp* step = NULL;
        s = new ForStatement(start, end, step, vdl, tb);
    } 
    return s;
}

Exp* Parser::parseCExp(){
    Exp* left = parseExpression();
    if (match(Token::LT) || match(Token::LE) || match(Token::EQ) || match(Token::GE) || match(Token::GT)){
        BinaryOp op;
        if (previous->type == Token::LT){
            op = LT_OP;
        }
        else if (previous->type == Token::LE){
            op = LE_OP;
        }
        else if (previous->type == Token::EQ){
            op = EQ_OP;
        }
        else if (previous->type == Token::GE){
            op = GE_OP;
        }
        else if (previous->type == Token::GT){
            op = GT_OP;
        }
        Exp* right = parseExpression();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseExpression() {

    Exp* left = parseTerm();
    while (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp op;
        if (previous->type == Token::PLUS){
            op = PLUS_OP;
        }
        else if (previous->type == Token::MINUS){
            op = MINUS_OP;
        }
        Exp* right = parseTerm();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseTerm() {
    Exp* left = parseFactor();
    while (match(Token::MUL) || match(Token::DIV)) {
        BinaryOp op;
        if (previous->type == Token::MUL){
            op = MUL_OP;
        }
        else if (previous->type == Token::DIV){
            op = DIV_OP;
        }
        Exp* right = parseFactor();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseFactor() {
    Exp* e;
    Exp* e1;
    Exp* e2;
    cout << previous->text << endl;
    if (match(Token::NUM)) {
        cout << previous->text << " ";
        return new NumberExp(stoi(previous->text));
    }
    else if (match(Token::ID)) {
        cout << previous->text << " ";
        string texto = previous->text;
        //Parse FCallExp
        if (match(Token::PI)){
            list<Exp*> args;
            if (!check(Token::PD)){
                args.push_back(parseCExp());
                while (match(Token::COMA)){
                    args.push_back(parseCExp());
                }
            }
            if (!match(Token::PD)){
                cout << "Falta paréntesis derecho" << endl;
                exit(0);
            }
            return new FCallExp(texto, args);
        }
        else{
            return new IdentifierExp(previous->text);
        }
            
    } else if (match(Token::PI)){
        cout << previous->text << " ";
        e = parseCExp();
        if (!match(Token::PD)){
            cout << "Falta paréntesis derecho" << endl;
            exit(0);
        }
        cout << previous->text << " ";

        return e;
    }
    
    cout << "Error: se esperaba un número o identificador." << endl;
    exit(0);
}

