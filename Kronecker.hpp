#pragma once
#include <Eigen/Dense>
namespace userdefined
{
	inline Eigen::MatrixXcd Kronecker(const Eigen::MatrixXcd& A,const Eigen::MatrixXcd& B) 
	{
		int Arows=A.rows(),Acols=A.cols(),Brows=B.rows(),Bcols=B.cols();
		Eigen::MatrixXcd C(Arows*Brows,Acols*Bcols);
		for(int i=0;i<Arows;i++) 
		{
            		for(int j=0;j<Acols;j++)
                		C.block(i*Brows,j*Bcols,Brows,Bcols)=A(i,j)*B;
        	}
        return C;
	}
}
