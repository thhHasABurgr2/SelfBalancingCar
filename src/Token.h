#include <string>
#include<iostream>
<<<<<<< HEAD
#include<memory>
=======
>>>>>>> e765849e95c897eb133c26dfce7e26a3bf08c7af
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
<<<<<<< HEAD
    q6,// symbol
    q7, // (
    q8, // )
    q9,//=
    qMatrix,// matrix state
=======
    q6,// &
    q7,// "\"
    q8, // (
    q9, // )
    q10,//=
    q11, // .
    qA,//A matrix
    qB,//B matrix
    qC,//C matrix
>>>>>>> e765849e95c897eb133c26dfce7e26a3bf08c7af
    qT, // T(period)
    qerr,//dont recognize or no DSA path for nonsense input
};












enum class Lexeme{
    FLAG,// stuff that start with !
    IDENTIFIER,// variables, symbolic or not
    NUMBER,//floats and numbers
<<<<<<< HEAD
    OPERATION, //  +,-,*,/, 
    SYMBOL,// &,//
    MATRIX// A, B, and C
    T,// period
    EQUAL,// =
    SEMICOLON,// ;
    FRONTPAREN,//(
    BACKPAREN,//)
=======
    OPERATION, //  +,-,*,/, ()
    SYMBOL,// &,//
    A,// A matrix
    B,// B matrix
    C,// C matrix
    T,// period
    EQUAL,// =
    SEMICOLON,// ;
>>>>>>> e765849e95c897eb133c26dfce7e26a3bf08c7af
    UNKNOWN,
 
};

class Token{
    private:
    std::string content;
    Lexeme lexeme;
    bool finalized;
<<<<<<< HEAD
    std::unique_ptr<Token> nextToken;
    State currState;
    bool flag;
    public:

    Token(): content(""), lexeme(Lexeme::UNKNOWN),finalized(false),nextToken(nullptr),currState(State::q0),flag(false){}
    Token(const char* cont, Lexeme lex): content(cont), lexeme(lex), finalized(false),nextToken(nullptr),currState(State::q0),flag(false){}
=======
    Token* nextToken;
    State currState;
    public:

    Token(): content(""), lexeme(Lexeme::UNKNOWN),finalized(false),nextToken(nullptr),currState(State::q0){}
>>>>>>> e765849e95c897eb133c26dfce7e26a3bf08c7af
    std::string getContent() const{
        return content;
    }
    Lexeme getLexeme() const{
        return lexeme;
    }
    Token* getNext() const{
<<<<<<< HEAD
        return nextToken.get();
=======
        return nextToken;
>>>>>>> e765849e95c897eb133c26dfce7e26a3bf08c7af
    }
    State getState() const{
        return currState;
    }
<<<<<<< HEAD
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
=======
    void append(char input){
        content+=input;
    }
    void next(Token* nextTok){
        nextToken=nextTok;
>>>>>>> e765849e95c897eb133c26dfce7e26a3bf08c7af
    }


};


#endif