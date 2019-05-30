#include <vector>
#include <iostream>

#include "SparseMatrix.h"
#include "SparseMatrixException.h"

template <typename T>
SparseMatrix<T>::SparseMatrix(const int & n)
{
    construct(n,n);
}

template <typename T>
SparseMatrix<T>::SparseMatrix(const int & row, const int & col)
{
    construct(row,col);
}

template <typename T>
SparseMatrix<T>::SparseMatrix(const SparseMatrix<T> & mat)
{
    deepCopy(mat);
}

template <typename T>
void SparseMatrix<T>::operator = (const SparseMatrix<T> & mat)
{
    deepCopy(mat);
}

template <typename T>
void SparseMatrix<T>::construct(const int & row, const int & col)
{
    if (row < 1 || col < 1)
    {
        throw InvalidDimensionException("Dimensions greater than 0 required.");
    }

    m = row;
    n = col;
}

template <typename T>
void SparseMatrix<T>::insert(const int & row, const int & col, const T & val)
{
    rows.push_back(row);
    cols.push_back(col);
    vals.push_back(val);
}

template <typename T>
void SparseMatrix<T>::checkBound(const int & row, const int & col) const
{
    if ((row > m-1) || (col > n-1))
    {
        throw BoundException("Index out of bound.");
    }
}

template <typename T>
void SparseMatrix<T>::set(const int & row, const int & col, const T & val)
{
    checkBound(row,col);

    // Check whether desired index already has NZ val
    int element_idx = -1;
    if (findIndex(row,col,element_idx))
    { 
        vals[element_idx] = val;
    }
    else
    {
        insert(row,col,val);
    }
}

template <typename T>
bool SparseMatrix<T>::findIndex(const int & row, const int & col, int & idx) const
{
    for (int i=0; i<rows.size(); i++)
    {
        if (rows[i] == row && cols[i] == col)
        {
            idx = i;
            return true; // Success
        }
    }

    return false; // No NZ element found
}

template <typename T>
T SparseMatrix<T>::get(const int & row, const int & col) const
{
    checkBound(row,col);

    int element_idx = -1;
    if (findIndex(row,col,element_idx))
    {
        return vals[element_idx];
    }

    return T(); // return 0
}

template <typename T>
int SparseMatrix<T>::getRowSize(void) const
{
    return m;
}

template <typename T>
int SparseMatrix<T>::getColSize(void) const
{
    return n;
}

template <typename T>
void SparseMatrix<T>::deepCopy(const SparseMatrix<T> & mat)
{
    m = mat.m;
    n = mat.n;

    // Copy elements from m
    if (!mat.vals.empty())
    {
        rows = mat.rows;
        cols = mat.cols;
        vals = mat.vals;
    }
}

template <typename T>
void SparseMatrix<T>::multiply(const std::vector<T> & v)
{
    std::vector<T> out = (*this) * v;

    // Pick NZ elements from output 
    int pos_cnt = 0;
    for (int i=0; i<m; i++)
    {
        if (out[i] > T())
        {
            rows[pos_cnt] = i;
            cols[pos_cnt] = 0;
            vals[pos_cnt++] = out[i];
        }
    }

    // Resize storage containers
    rows.resize(pos_cnt);
    cols.resize(pos_cnt);
    vals.resize(pos_cnt);

    // Adjust column dimension
    n = 1;
}

template <typename T>
void SparseMatrix<T>::operator *= (const std::vector<T> & v) 
{
    multiply(v);
}

template <typename T>
std::vector<T> SparseMatrix<T>::operator * (const std::vector<T> & v) const
{
    if (n != int(v.size()))
    {
        throw InvalidDimensionException("Inner dimensions do not agree.");
    }

    std::vector<T> out(m,T());
    if (!vals.empty()) // This is not all zeroes
    {
        for (int i=0; i<vals.size(); i++)
        {
            out[rows[i]] += vals[i]*v[cols[i]];
        }
    }

    return out;
}

template <typename T>
void SparseMatrix<T>::operator *= (const SparseMatrix<T> & mat)
{
    multiply(mat);
}

template <typename T>
void SparseMatrix<T>::operator += (const SparseMatrix<T> & mat)
{
    plus(mat);
}

template <typename T>
void SparseMatrix<T>::operator -= (const SparseMatrix<T> & mat)
{
    minus(mat);
}

template <typename T>
void SparseMatrix<T>::multiply(const SparseMatrix<T> & mat)
{
    if (n != mat.m)
    {
        throw InvalidDimensionException("Inner dimensions do not agree.");
    }

    SparseMatrix<T> result(m,mat.n);
    T sum;
    for (int i=0; i<m; i++)
    {
        for (int j=0; j<mat.n; j++)
        {
            sum = T();
            for (int k=0; k<n; k++)
            {
                sum += get(i,k)*mat.get(k,j);
            }

            if (sum > T() || sum < T())
                result.insert(i,j,sum);
        }
    }
    
    n = mat.n;
    rows = result.rows;
    cols = result.cols;
    vals = result.vals;
}

template <typename T>
void SparseMatrix<T>::dot(const SparseMatrix<T> & m)
{
    multiply(m);
}

template <typename T>
SparseMatrix<T> SparseMatrix<T>::operator * (const SparseMatrix<T> & m) const
{
    SparseMatrix<T> out = *this;
    out.multiply(m);

    return out;
}

template <typename T>
void SparseMatrix<T>::transpose(void)
{
    std::vector<int> tmp = rows;
    rows = cols;
    cols = tmp;
}

template <typename T>
void SparseMatrix<T>::plus(const SparseMatrix<T> & mat)
{
    if (mat.m != m || mat.n != n)
    {
        throw InvalidDimensionException("Dimensions do not agree.");
    }

    SparseMatrix<T> result(m,n);
    T sum;
    for (int i=0; i<m; i++)
    {
        sum = T();
        for (int j=0; j<n; j++)
        {
            sum = get(i,j) + mat.get(i,j);

            if (sum > T() || sum < T())
                result.set(i,j,sum);
        }
    }

    rows = result.rows;
    cols = result.cols;
    vals = result.vals;
}

template <typename T>
void SparseMatrix<T>::minus(const SparseMatrix<T> & mat)
{
    if (mat.m != m || mat.n != n)
    {
        throw InvalidDimensionException("Dimensions do not agree.");
    }

    SparseMatrix<T> result(m,n);
    T diff;
    for (int i=0; i<m; i++)
    {
        diff = T();
        for (int j=0; j<n; j++)
        {
            diff = get(i,j) - mat.get(i,j);

            if (diff > T() || diff < T())
                result.set(i,j,diff);
        }
    }

    rows = result.rows;
    cols = result.cols;
    vals = result.vals;
}

template <typename T>
SparseMatrix<T> SparseMatrix<T>::operator + (const SparseMatrix<T> & mat) const
{
    SparseMatrix<T> result(*this);
    result.plus(mat);
    return result;
}

template <typename T>
SparseMatrix<T> SparseMatrix<T>::operator - (const SparseMatrix<T> & mat) const
{
    SparseMatrix<T> result(*this);
    return result.minus(mat);
}

template <typename T>
void SparseMatrix<T>::printMatrix(void) const
{
    for (int i=0; i<m; i++)
    {
        for (int j=0; j<n; j++)
        {
            std::cout << get(i,j) << " ";
        }
        std::cout << std::endl;
    }
}

