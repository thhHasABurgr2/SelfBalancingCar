#ifndef Matrix_h
#define Matrix_h
#include <string>
/*
fast, compile time Matrix
pretty much a more ligthweight version of SymMatrix
*/


template<uint8_t row,uint8_t col> class Matrix{
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
Matrix<r,c> matmul(const Matrix<rowLHS,colLHS>& lhs, const Matrix<rowRHS,colRHS>& rhs){
    
    
    
    static_assert(colLHS==rowRHS,"dimension mismatch");

    
    Matrix<rowLHS,colRHS> product;
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



    

