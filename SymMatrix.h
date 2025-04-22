#ifndef SymMatrix_h
#define SymMatrix_h
#include "SymMatrix.h"
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
template struct Input{
    String symValue
    float  numValue
}

template<uint8_t row,uint8_t col> class SymMatrix{
    private:
    float matrix[row][col];
    public:
    void initialize(){
        for(uint8_t i=1; i<=row;i++){
            for(uint8_t j=1; j<=col;j++){
                matrix[i-1][j-1]=0;
            }
        }

    }

    
    void set(uint8_t row, uint8_t col, float num){
        matrix[row-1][col-1]=num;
    }
    float getElem(uint8_t row, uint8_t col){
        return matrix[row-1][col-1];
    }
    static constexpr uint8_t rows() { 
        return row; 
    }
    static constexpr uint8_t cols() { 
        return col; 
    }
};


template<uint8_t r,uint8_t c,uint8_t rowLHS,uint8_t rowRHS,uint8_t colLHS,uint8_t colRHS>
SymMatrix<r,c> matmul(const SymMatrix<rowLHS,colLHS>& lhs, const SymMatrix<rowRHS,colRHS>& rhs){
    
    
    
    static_assert(colLHS==rowRHS,"dimension mismatch");

    
    SymMatrix<rowLHS,colRHS> product;
    for(uint8_t i=1; i<=rowLHS;i++){
        for(uint8_t j=1; j<=colRHS;j++){
            float sum=0;
            for(uint8_t k=1;k<=rowRHS;k++){
                sum+=lhs.getElem(i,k) * rhs.getElem(k,j);
            }
            product.set(i,j,sum);

        }
    }
    return product;
}


#endif