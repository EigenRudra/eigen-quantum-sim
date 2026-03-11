#pragma once
#include <Eigen/Sparse>
#include <vector>
namespace userdefined
{
	typedef Eigen::SparseMatrix<std::complex<double>> Mtrx;
	typedef Eigen::Triplet<std::complex<double>> T;

	inline Mtrx Kronecker(const Mtrx& A,const Mtrx& B) 
	{
		std::vector<T> list;	//Triplet list for storing sparse matrix data
		list.reserve(A.nonZeros()*B.nonZeros());

		for(int i=0;i<A.outerSize();i++)	//Loops through columns of A
			for(Mtrx::InnerIterator j(A,i);j;++j)	//Loops ONLY through non zero elements of A
				for(int k=0;k<B.outerSize();k++)	//Similarly for B
					for(Mtrx::InnerIterator l(B,k);l;++l)
						list.push_back(T( j.row()*B.rows()+l.row() , j.col()*B.cols()+l.col() , j.value()*l.value() ));

		Mtrx C(A.rows()*B.rows(),A.cols()*B.cols());
		C.setFromTriplets(list.begin(),list.end());
		return C;
	}
}
