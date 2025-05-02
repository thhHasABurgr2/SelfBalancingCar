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
        for(uint8_t i=0; i<row;i++){
            for(uint8_t j=0; j<col;j++){
                matrix[i][j]=0.0f;
            }
        }

    }

    
    void set(uint8_t row, uint8_t col, float num){
        matrix[row][col]=num;
    }
    float getElem(uint8_t row, uint8_t col){
        return matrix[row][col];
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
    for(uint8_t i=0; i<rowLHS;i++){
        for(uint8_t j=0; j<colRHS;j++){
            float sum=0.0f;
            for(uint8_t k=0;k<rowRHS;k++){
                sum+=lhs.getElem(i,k) * rhs.getElem(k,j);
            }
            product.set(i,j,sum);

        }
    }
    return product;
}
#endif



    

