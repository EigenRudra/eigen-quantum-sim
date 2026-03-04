#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <random>
#include <iomanip>
#include <bitset>
#include <Eigen/Dense>
#include "Kronecker.hpp" 

using Eigen::Matrix2cd;
using Eigen::MatrixXcd;
using Eigen::VectorXcd;
using namespace std::complex_literals;

class Quantum
{
private:
	int n,dim;
	VectorXcd state;
	Matrix2cd H,X,Y,Z,S,T,I,P0,P1; //Last 2 are projectors |0><0| & |1><1|
    
	MatrixXcd expand(const Matrix2cd& gate,int tgt)	//To expand 2x2 gate for 2^n Hilbert space
	{
		MatrixXcd op;
		for(int i=0;i<n;i++)
		{
			MatrixXcd temp=(i==tgt)?gate:I;
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
		state=expand(op,t)*state;
	}
	
	//CNOT gate
	void cnot(int control,int target) 
	{
        	MatrixXcd t1,t2,op1,op2;
        	for(int i=0;i<n;i++)
        	{
			op1=op2=I;
			if(i==control)
			{
				op1=P0;
				op2=P1;
			}
			else if(i==target)
				op2=X;
				
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
		MatrixXcd cnot=t1+t2;
		state=cnot*state;
	}

	
	void measure()
	{
		std::vector<double> p(dim);	//Storing the norm (aka prob)
		int i,bars,hash;
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
			std::string bin=std::bitset<16>(i).to_string().substr(16-n);
			prob=p[i]*100.0;
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

		std::cout<<"Collapsed to state: |"<<std::bitset<16>(ms).to_string().substr(16-n)<<">\n";
		state=VectorXcd::Zero(dim);
		state(ms)=1.0;
	}

	void print()
	{
		std::cout << "\nCurrent State Vector:\n" << state << "\n";
	}
};//end of class

int main() 
{    
	int n;
	std::cout<<"Enter no.of qubits (1-10): ";
	std::cin>>n;
	if(n<=0||n>10)
	{
		std::cout<<"Invalid Input! No.of qubits should be between 1-10\n";
		return 1;
	}

	Quantum q(n);
	std::cout<<"Initialized "<<n<<"-qubit environment in state |0...0>\n";
	std::cout<<"Available Commands:\n";
	std::cout<<"  h <target>          (e.g: h 0)\n";
	std::cout<<"  x <target>          (e.g: x 1)\n";
	std::cout<<"  y <target>          (e.g: y 1)\n";
	std::cout<<"  z <target>          (e.g: z 0)\n";
	std::cout<<"  s <target>          (e.g: s 2)\n";
	std::cout<<"  t <target>          (e.g: t 0)\n";
	std::cout<<"  cnot <ctrl> <tgt>   (e.g: cnot 0 1)\n";
	std::cout<<"  state               (Prints current state vector)\n";
	std::cout<<"  measure             (Collapses wavefunction and exits)\n";
	std::cout<<"  exit                (Quits simulator)\n";

	std::string cmd;
	do
	{
		std::cout<<"QuantumShell> "<<std::flush;
		std::cin>>cmd;
		
		if(cmd=="exit")
			break;
		else if(cmd=="measure")
		{
			q.measure();
			break;
		}
		else if(cmd=="state")
			q.print();
		else if(cmd=="cnot")
		{
			int c,t;
			std::cin>>c>>t;
			if(c>=0 && c<n && t>=0 && t<n && c!=t)
				q.cnot(c,t);
			else
				std::cout<<"Error! Invalid control or target qubit\n";
		}
		else if(cmd=="h"||cmd=="x"||cmd=="y"||cmd=="z"||cmd=="s"||cmd=="t")
		{
			int t;
			std::cin>>t;
			if(t>=0 && t<n)
				q.apply(cmd,t);
			else
				std::cout<<"Error! Invalid target qubit\n";
		}
		else
		{
			std::cout<<"Error! Unknown command\n";
			std::cin.clear();
			std::cin.ignore(10000,'\n');
		}
	}while(true);
	return 0;
}
