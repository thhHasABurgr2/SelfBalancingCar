#ifndef SymMatrix_h
#define SymMatrix_h
#include <string>
#include "Matrix.h"
#include <iostream>
/*

Here's my idea: 

U have a file for input for the equation.
Parses the numbers as floats and gets the parse stack for the symbolic matrix
inspired by Latex format
for instance, in the input file: A=\begin a&b\\c&d;
semicolon denotes end (so that I dont have to worry abt matching curly braces)
& refers to tab just like in latex, and \\ refers to newline
'\begin' to start matrix

using a template sctruct for input and mapping for symbolic value to float




*/
enum class Type{
    CONST,
    SYM
};

class Variable{
    private:
    float numValue;
    std::string symValue;
    Type type;
    public:
    Variable(float numValue_,std::string symValue_){
        numValue=numValue_;
        symValue=symValue_;
        if(symValue_=="")type=Type::CONST; else type =Type::SYM;
    }
    Variable(float numValue_) : Variable(numValue_, "") {}// constructor delegation


    //getters and setters
    void setVar(std::string var){
        symValue=var;
    }
    void setVal(float val){
        numValue=val;
    }
    std::string getType ()const{
        return type==Type::CONST ? "CONST" : "SYMBOLIC";
        
    }
    std::string getVar ()const{
        return (type==Type::CONST) ? std::to_string(numValue): symValue;
    }
    float getVal ()const{
        return numValue;
    }
    //basic arithmetic

    Variable add (const Variable& var)const{;//wait this tracks changes made to a nameless variable too. thats cool
        return Variable(this->numValue+var.getVal(),this->symValue+" + "+var.getVar());
    }
    Variable operator+ (const Variable& var)const{
        return this->add(var);
    }
    Variable subtract (const Variable& var)const{;//wait this tracks changes made to a nameless variable too. thats cool
        return Variable(this->numValue-var.getVal(),this->symValue+" - "+var.getVar());
    }
    Variable operator-(const Variable& var)const{
        return this->subtract(var);
    }
    Variable multiply (const Variable& var)const{;//wait this tracks changes made to a nameless variable too. thats cool
        return Variable(this->numValue*var.getVal(),this->symValue+" * "+var.getVar());
    }
    Variable operator* (const Variable& var)const{
        return this->multiply(var);
    }
    Variable divide (const Variable& var)const{;//wait this tracks changes made to a nameless variable too. thats cool
        float result= (var.getVal()!=0.0f) ?  (this->numValue/var.getVal()) : 0.0f;
        return Variable( result,this->symValue+(" / "+var.getVar()));
    }
    Variable operator/ (const Variable& var)const{
        return this->divide(var);
    }

};

struct Element{
    /*
    ParseCode instructions
    std::vector<Variable> dependentParams
    
    */
};

template<uint8_t row,uint8_t col> class SymMatrix{
    private:
    Element matrix[row][col];
    uint8_t countVars=0;
    public:

    void initialize(){
        
        for(uint8_t i=0; i<row;i++){
            for(uint8_t j=0; j<col;j++){
                matrix[i][j]= new Element(Type::CONST,"",0.0);
            }
        }
    }
    SymMatrix(){
        initialize();
    }
    

    
    void set(uint8_t row, uint8_t col, Element* num){
        matrix[row-1][col-1]=num;
        if(num->type==Type::SYM) countVars++;
    }
    Element* getElem(uint8_t row, uint8_t col){
        return matrix[row-1][col-1];
    }
    static constexpr uint8_t rows() { 
        return row; 
    }
    static constexpr uint8_t cols() { 
        return col; 
    }
    Matrix<row,col>* eval(){
        Matrix<row,col>
    }

};





#endif