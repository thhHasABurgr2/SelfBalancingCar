#include "Token.h"
#include "Line.h"

namespace imp{
    bool isDigit(char input) {
        return (input<=57 &&input>=48);
    }
    bool isLetter(char input) {
        return (input >= 97 && input <=122);
    }
    bool isOperator(char input) {
        return (input=='+'||input=='-'||input=='*'||input=='/');
    }
    bool isSemicolon(char input) {
        return (input == ';');
    }
    bool isEqual(char input) {
        return (input == '=');
    }
    bool isDot(char input) {
        return (input =='.');
    }
    bool isFlag(char input) {
        return (input =='!');
    }
    bool isSymbol(char input) {
        return (input =='&' || input=='\\');
    }

    bool isAmbersand(char input) {
        return (input =='&');
    }
    bool isBackslash(char input) {
        return (input =='\\');
    }
    bool isFrontParen(char input) {
        return (input =='(');
    }
    bool isBackParen(char input) {
        return (input ==')');
    }
    bool isMatrix(char input) {
        return (input=='A'||input=='B'||input=='C');
    }
    bool isT(char input) {
        return (input=='T');
    }
    /*
example input
!variables;
a=3.4;
b=23.3;
!end;
!symbolic;
c=4;
!end;
!matrix;
A= a*b+c & c\\ (c+2.3)*b;
B= a*b+c & c\\ (c+2.3)*b;
C= a*b+c & c+9\\ (c+2.3)*b;
!end;
!period;
T=40;
!end;


*/
std::unique_ptr<Token> TokensPerLine(Line* input){
        auto head = std::make_unique<Token>();
        Token* currentToken=head.get();
        std::string content=input->getContent();
        int i=0;
        char inputc=content[i];
        while(i<content.length()){//cond seems weird but
            //if I can keep doing currentToken->next() b4 the loop evals its condition then I can stop at the last token
            
                if(currentToken->getState()==State::q0){
                    if(isFlag(inputc)){
                        currentToken->state(State::q3);
                        
                    }
                    else if(isDigit(inputc)){
                        currentToken->state(State::q2);
                    }
                    else if(isLetter(inputc)){
                        currentToken->state(State::q1);
                       
                    }
                    else if(isMatrix(inputc)){
                        currentToken->state(State::qMatrix);
                    }
                   
                    else if(isT(inputc)){
                        currentToken->state(State::qT);
                    }
                    else if(isFrontParen(inputc)){
                        currentToken->state(State::q7);
                    }else if(isEqual(inputc)){
                        currentToken->state(State::q9);
                    }
                    else{
                        currentToken->state(State::qerr);
                    }
                    
                }
                else if(currentToken->getState()==State::q1){
                    currentToken->append(inputc);
                    currentToken->setLexeme(Lexeme::IDENTIFIER);
                    std::cout << "Processing char: " << inputc << " at i = " << i << std::endl;
                    currentToken->printTokens();
                    i++;
                    inputc=content[i];
                    
                    
                    if(isLetter(inputc)){
                        continue;
                    }
                    else if (isOperator(inputc)){
                        currentToken->state(State::q4);
                    }
                    else if(isSymbol(inputc)){
                        currentToken->state(State::q6);
                    }
                    else if(isBackParen(inputc)){
                        currentToken->state(State::q8);
                    }
                    else if(isEqual(inputc)){
                        currentToken->state(State::q9);
                    }else{
                        currentToken->state(State::qerr);
                    }
                }
                else if(currentToken->getState()==State::q2){
                    currentToken->setLexeme(Lexeme::NUMBER);
                    if(isDigit(inputc)){
                        currentToken->append(inputc);
                        i++;
                        inputc=content[i];
                    }
                    else if(isDot(inputc)){
                        currentToken->append(inputc);
                        i++;
                        inputc=content[i];
                    }
                    else if(isSemicolon(inputc)){
                        currentToken->state(State::q5);
                    }
                    else if(isBackParen(inputc)){
                        currentToken->state(State::q8);
                    }
                    else if(isOperator(inputc)){
                        currentToken->state(State::q4);
                    }
                    else if(isSymbol(inputc)){
                        currentToken->state(State::q6);
                    }else{
                        currentToken->state(State::qerr);
                    }
                }
                    
                else if(currentToken->getState()==State::q3){
                    currentToken->flago();
                    currentToken->setLexeme(Lexeme::FLAG);
                    currentToken->append(inputc);
                    i++;
                    inputc=content[i];
                    if(isLetter(inputc)){
                        continue;
                    }else if(isSemicolon(inputc)){
                        currentToken->state(State::q5);
                    }else{
                        currentToken->state(State::qerr);
                    }
                }
                        
                else if(currentToken->getState()==State::q4){
                    currentToken->finalize();
                    std::cout << "Processing char: " << inputc << " at i = " << i << std::endl;
                    currentToken->printTokens();

                    currentToken->next(std::make_unique<Token>(std::string(1,inputc).c_str(),Lexeme::OPERATION));// expecting an operaition rn
                    currentToken = currentToken->getNext();
                    currentToken->finalize();
                    currentToken->printTokens();
                    currentToken->next(std::make_unique<Token>());
                    currentToken = currentToken->getNext();
                    i++;
                    inputc=content[i];
                    currentToken->printTokens();
                }
                else if(currentToken->getState()==State::q5){
                    currentToken->finalize();
                    currentToken->next(std::make_unique<Token>(std::string(1,inputc).c_str(),Lexeme::SEMICOLON));// expecting an operaition rn
                    currentToken = currentToken->getNext();
                    currentToken->finalize();
                    currentToken->next(std::make_unique<Token>());
                    currentToken = currentToken->getNext();
                    i++;
                    inputc=content[i];
                }
                else if(currentToken->getState()==State::q6){
                    currentToken->finalize();
                    currentToken->next(std::make_unique<Token>(std::string(1,inputc).c_str(),Lexeme::SYMBOL));// expecting an operaition rn
                    currentToken = currentToken->getNext();
                    currentToken->finalize();
                    currentToken->next(std::make_unique<Token>());
                    currentToken = currentToken->getNext();
                    i++;
                    inputc=content[i];
                }
                else if(currentToken->getState()==State::q7){
                    currentToken->finalize();
                    currentToken->next(std::make_unique<Token>(std::string(1,inputc).c_str(),Lexeme::FRONTPAREN));// expecting an operaition rn
                    currentToken = currentToken->getNext();
                    currentToken->finalize();
                    currentToken->next(std::make_unique<Token>());
                    currentToken = currentToken->getNext();
                    i++;
                    inputc=content[i];
                }
                else if(currentToken->getState()==State::q8){
                    currentToken->finalize();
                    currentToken->next(std::make_unique<Token>(std::string(1,inputc).c_str(),Lexeme::BACKPAREN));// expecting an operaition rn
                    currentToken = currentToken->getNext();
                    currentToken->finalize();
                    currentToken->next(std::make_unique<Token>());
                    currentToken = currentToken->getNext();
                    i++;
                    inputc=content[i];
                }
                else if(currentToken->getState()==State::q9){
                    currentToken->finalize();
                    currentToken->next(std::make_unique<Token>(std::string(1,inputc).c_str(),Lexeme::EQUAL));// expecting an operaition rn
                    currentToken = currentToken->getNext();
                    currentToken->finalize();
                    currentToken->next(std::make_unique<Token>());
                    currentToken = currentToken->getNext();
                    i++;
                    inputc=content[i];
                }
                else if(currentToken->getState()==State::qMatrix){    
                    currentToken->setLexeme(Lexeme::MATRIX);
                    currentToken->append(inputc);
                    currentToken->finalize();
                    std::cout << "Processing char: " << inputc << " at i = " << i << std::endl;
                    currentToken->printTokens();

                    i++;
                    inputc=content[i];
                    std::cout << "Processing char: " << inputc << " at i = " << i << std::endl;

                    if(isEqual(inputc)){
                        currentToken->next(std::make_unique<Token>(std::string(1,inputc).c_str(),Lexeme::EQUAL));
                        currentToken = currentToken->getNext();
                        currentToken->finalize();
                        currentToken->printTokens();
                        currentToken->next(std::make_unique<Token>());
                        currentToken = currentToken->getNext();
                        i++;
                        inputc=content[i];  
                        std::cout << "Processing char: " << inputc << " at i = " << i << std::endl;
    
                    }else{
                        currentToken->state(State::qerr);
                    }
                    currentToken->printTokens();
                                     
                   
                }
                else if(currentToken->getState()==State::qT){   
                    currentToken->setLexeme(Lexeme::T);
                    currentToken->append(inputc);
                    i++;
                    inputc=content[i];
                    currentToken->finalize();
                    currentToken->next(std::make_unique<Token>());
                    currentToken = currentToken->getNext();
                }
                
                else if(currentToken->getState()==State::qerr){
                    std::cout << "bad char "+std::string(1,inputc)<<std::endl;
                    std::cout <<"stuff inside currentToken: "+currentToken->getContent()<<std::endl;
                    exit(1);
                }
        }
        return head;
    }
    

}

