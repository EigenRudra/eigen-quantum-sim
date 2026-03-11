#pragma once
#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <random>
#include <iomanip>
#include <bitset>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include "Kronecker.hpp" 

using Eigen::Matrix2cd;
using Eigen::VectorXcd;
using namespace std::complex_literals;
typedef Eigen::SparseMatrix<std::complex<double>> Mtrx;

class Quantum
{
private:
	int n,dim;
	VectorXcd state;
	Matrix2cd H,X,Y,Z,S,T,I,P0,P1; //Last 2 are projectors |0><0| & |1><1|
    
	Mtrx expand(const Matrix2cd& dgate,int tgt)	//To expand 2x2 gate for 2^n Hilbert space, 
	{						//dgate denotes dense matrix version of gate
		Mtrx op, gate=dgate.sparseView(), spI=I.sparseView();	//spI denotes sparse version of I matrix
		for(int i=0;i<n;i++)
		{
			Mtrx temp=(i==tgt)?gate:spI;
			if(i==0)
				op=temp;
			else
				op=userdefined::Kronecker(op,temp);
		}
		return op;
	}

public:
	Quantum(int n)
	{
		this->n=n;
		dim=1<<n;	//Using bitwise to avoid for eg 2^3=7.9999=7 (due to int)
        	state=VectorXcd::Zero(dim);
        	state(0)=1.0;
        	const double x= 1.0/std::sqrt(2.0);

		H<<x,x,x,-x;
		X<<0,1,1,0;
		Y<<0,-1i,1i,0;
		Z<<1,0,0,-1;
		S<<1,0,0,1i;
		T<<1,0,0,std::complex<double>(x,x);
		I=Matrix2cd::Identity();
		P0<<1,0,0,0;
		P1<<0,0,0,1;
	}
	
	void apply(const std::string& gate,int t)
	{
		Matrix2cd op;
		if(gate=="h") 
			op=H;
		else if(gate=="x")
			op=X;
		else if(gate=="y")
			op=Y;
		else if(gate=="z")
			op=Z;
		else if(gate=="s")
			op=S;
		else if(gate=="t")
			op=T;
		state=expand(op,t)*state;	//Sparse Matrix*Dense vector
	}
	
	//CNOT gate
	void cnot(int control,int target) 
	{
        	Mtrx t1,t2,op1,op2, spI=I.sparseView(), spP0=P0.sparseView(), spP1=P1.sparseView(), spX=X.sparseView();
        	for(int i=0;i<n;i++)
        	{
			op1=op2=spI;
			if(i==control)
			{
				op1=spP0;
				op2=spP1;
			}
			else if(i==target)
				op2=spX;
				
			if(i==0)
			{
				t1=op1;
				t2=op2;
			}
			else
			{
				t1=userdefined::Kronecker(t1,op1);
				t2=userdefined::Kronecker(t2,op2);
			}
		}
		Mtrx cnot=t1+t2;
		state=cnot*state;
	}

	
	void measure()
	{
		std::vector<double> p(dim);	//Storing the norm (aka prob)
		int i,hash;
		double prob;
		std::cout << "\n--- Quantum Measurement ---\n";
		for(i=0;i<dim;i++)
			p[i]=std::norm(state(i));

		std::random_device rd;
		std::mt19937 gen(rd());
		std::discrete_distribution<> d(p.begin(),p.end());
		int ms=d(gen);	//Random number for measured state
		
		//Printing a hroizontal histogram for visualization
		for(i=0;i<dim;i++)
		{
			prob=p[i]*100.0;
			if(prob>0.01)
			{
				std::string bin=std::bitset<32>(i).to_string().substr(32-n);
				std::cout<<"|"<<bin<<">: [";
				hash=static_cast<int>(prob/5.0);
            			for(int j=0;j<20;j++) 
            			{
                			if(j<hash) 
                				std::cout<<"#";
                			else 
                				std::cout<<" ";
            			}
            			std::cout<<"] "<<std::fixed<<std::setprecision(2)<<prob<<"%\n";
        		}
		}
		std::cout<<"Collapsed to state: |"<<std::bitset<32>(ms).to_string().substr(32-n)<<">\n";
		state=VectorXcd::Zero(dim);
		state(ms)=1.0;
	}

	void print()
	{
		std::cout<<"\nCurrent State Vector:\n";
		for(int i=0;i<dim;i++)
		{
			std::string bin=std::bitset<32>(i).to_string().substr(32-n);
			double r=state(i).real(), im=state(i).imag();
			std::cout<<" |"<<bin<<">  :  " <<std::fixed<<std::setprecision(4)<<r<<(im>=0?" + ":" - ")<<std::abs(im)<<"i\n";
		}
	std::cout << "\n";
	}
};//end of class
