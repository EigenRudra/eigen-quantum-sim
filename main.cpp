#include <iostream>
#include <string>
#include "Quantum.hpp"

int main() 
{    
	int n;
	std::cout<<"Enter no.of qubits (1-30): ";
	std::cin>>n;
	if(n<=0||n>30)
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
