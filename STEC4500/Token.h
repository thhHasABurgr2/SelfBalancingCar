#include <string>
#include<iostream>
#include<memory>
#ifndef TOKEN_H
#define TOKEN_H
/*
example input
!variables
a=3.4;
b=23.3;
!end
!symbolic
c=4;
!end
!matrix
A= a*b+c & c\\ (c+2.3)*b;
B= a*b+c & c\\ (c+2.3)*b;
C= a*b+c & c+9\\ (c+2.3)*b;
!end
!period
T=40;
!end


*/




enum class State{
    q0,//start
    q1,//letter
    q2,//number
    q3,//!
    q4,// operations:+,-,*,/
    q5,//;
    q6,// symbol
    q7, // (
    q8, // )
    q9,//=
    qMatrix,// matrix state
    qT, // T(period)
    qerr,//dont recognize or no DSA path for nonsense input
};












enum class Lexeme{
    FLAG,// stuff that start with !
    IDENTIFIER,// variables, symbolic or not
    NUMBER,//floats and numbers
    OPERATION, //  +,-,*,/, 
    SYMBOL,// &,//
    MATRIX// A, B, and C
    T,// period
    EQUAL,// =
    SEMICOLON,// ;
    FRONTPAREN,//(
    BACKPAREN,//)
    UNKNOWN,
 
};

class Token{
    private:
    std::string content;
    Lexeme lexeme;
    bool finalized;
    std::unique_ptr<Token> nextToken;
    State currState;
    bool flag;
    public:

    Token(): content(""), lexeme(Lexeme::UNKNOWN),finalized(false),nextToken(nullptr),currState(State::q0),flag(false){}
    Token(const char* cont, Lexeme lex): content(cont), lexeme(lex), finalized(false),nextToken(nullptr),currState(State::q0),flag(false){}
    std::string getContent() const{
        return content;
    }
    Lexeme getLexeme() const{
        return lexeme;
    }
    Token* getNext() const{
        return nextToken.get();
    }
    State getState() const{
        return currState;
    }
    bool isFinished() const{
        return finalized;
    }
    bool isFlag() const{
        return flag;
    }
    void append(char input){
        content+=input;
    }
    void next(std::unique_ptr<Token> nextTok){
        nextToken=std::move(nextTok);
    }
    void finalize(){
        finalized=true;
    }
    void state(State states){
        currState=states;
    }
    void flago(){
        flag=true;
    }
    void setLexeme(Lexeme lex){
        lexeme=lex;
    }


};


#endif