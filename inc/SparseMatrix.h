#include<vector>
#include<iostream>

#ifndef SPARSE_MATRIX_H_
#define SPARSE_MATRIX_H_

template <typename T>
class SparseMatrix
{
public:
    // Standard class functions
    SparseMatrix(const int & n); 
    SparseMatrix(const int & row, const int & col); 

    SparseMatrix(const SparseMatrix<T> & m); 
    void operator = (const SparseMatrix<T> & m); 

    ~SparseMatrix(void) {};

    // Getter & setter
    void set(const int & row, const int & col, const T & val); 
    T get(const int & row, const int & col) const; 
    int getRowSize(void) const; 
    int getColSize(void) const; 

    // Display
    void printMatrix(void) const;

    // Matrix manipulation functions
    void multiply(const SparseMatrix<T> & m); 
    void operator *= (const SparseMatrix<T> & m); 
    SparseMatrix<T> operator * (const SparseMatrix<T> & m) const; 
    void multiply(const std::vector<T> & v); 
    void operator *= (const std::vector<T> & v); 
    std::vector<T> operator * (const std::vector<T> & v) const; 

    void plus(const SparseMatrix<T> & m); 
    void operator += (const SparseMatrix<T> & m); 
    SparseMatrix<T> operator + (const SparseMatrix<T> & m) const;

    void minus(const SparseMatrix<T> & m); 
    void operator -= (const SparseMatrix<T> & m); 
    SparseMatrix<T> operator - (const SparseMatrix<T> & m) const;
    
    void dot(const SparseMatrix<T> & m); 
    void transpose(void); 

protected:
    int m, n;
    std::vector<int> rows, cols;
    std::vector<T> vals;

    // Helper functions
    void construct(const int & row, const int & col); 
    void deepCopy(const SparseMatrix<T> & m); 
    void insert(const int & row, const int & col, const T & val); 
    void checkBound(const int & row, const int & col) const; 
    bool findIndex(const int & row, const int & col, int & idx) const; 
};

#endif
