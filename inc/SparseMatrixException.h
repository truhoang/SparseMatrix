#include <exception>
#include <string>

#ifndef SPARSE_MATRIX_EXCEPTION_H_
#define SPARSE_MATRIX_EXCEPTION_H_

class SparseMatrixException : public std::exception
{
public:
	SparseMatrixException(const std::string & msg) : exception(), message(msg)
	{

	}

	virtual ~SparseMatrixException(void) throw()
	{

	}

	std::string getMessage(void) const
	{
		return message;
	}

protected:
	std::string message;
};

class InvalidDimensionException : public SparseMatrixException
{
public:
	InvalidDimensionException(const std::string & msg) : SparseMatrixException(msg)
	{

	}
};

class BoundException : public SparseMatrixException
{
public:
	BoundException(const std::string & msg) : SparseMatrixException(msg)
	{

	}
};

#endif
