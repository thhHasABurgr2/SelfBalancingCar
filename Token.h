#include <string>
#include<iostream>
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
    q6,// &
    q7,// "\"
    q8, // (
    q9, // )
    q10,//=
    q11, // .
    qA,//A matrix
    qB,//B matrix
    qC,//C matrix
    qT, // T(period)
    qerr,//dont recognize or no DSA path for nonsense input
};












enum class Lexeme{
    FLAG,// stuff that start with !
    IDENTIFIER,// variables, symbolic or not
    NUMBER,//floats and numbers
    OPERATION, //  +,-,*,/, ()
    SYMBOL,// &,//
    A,// A matrix
    B,// B matrix
    C,// C matrix
    T,// period
    EQUAL,// =
    SEMICOLON,// ;
    UNKNOWN,
 
};

class Token{
    private:
    std::string content;
    Lexeme lexeme;
    bool finalized;
    Token* nextToken;
    State currState;
    public:

    Token(): content(""), lexeme(Lexeme::UNKNOWN),finalized(false),nextToken(nullptr),currState(State::q0){}
    std::string getContent() const{
        return content;
    }
    Lexeme getLexeme() const{
        return lexeme;
    }
    Token* getNext() const{
        return nextToken;
    }
    State getState() const{
        return currState;
    }
    void append(char input){
        content+=input;
    }
    void next(Token* nextTok){
        nextToken=nextTok;
    }


};


#endif