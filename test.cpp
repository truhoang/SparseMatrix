#include "SparseMatrix.h"
#include "SparseMatrix.cpp"

int main(int argc, char const *argv[])
{
	SparseMatrix<float> m(3);
	std::cout << "Matrix declared" << std::endl;

	m.set(0,0,1);
	m.set(1,1,1);
	m.set(2,2,1);

	std::cout << "Set sucessfull" << std::endl;

	m.printMatrix();

	SparseMatrix<float> m2(m);

	std::cout << "Copy sucessfull" << std::endl;

	m2.printMatrix();

	SparseMatrix<float> m3(m+m2);

	std::cout << "Addition sucessfull" << std::endl;

	m3.printMatrix();

	m2 *= m3;

	std::cout << "Compound assignment sucessfull" << std::endl;

	m2.printMatrix();

	m.set(0,1,1);
	m.printMatrix();
	m.transpose();
	m.printMatrix();

	std::cout << "Transpose sucessfull" << std::endl;

	return 0;
}
