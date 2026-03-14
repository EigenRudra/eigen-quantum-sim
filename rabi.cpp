#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include "Quantum.hpp"

int main() 
{
	Quantum q(1);	//Initially perfectly spin-up |0>
	double freq,time,dt = 0.05,prob;
	int pos,width=50;	//For output formatting
	std::cout<<"Enter Magnetic Field Strength (Larmor Frequency): ";
	std::cin>>freq;
	std::cout<<"Enter Total Simulation Time (in seconds): ";
	std::cin>>time;
	std::cout<<"\nMagnetic Field turned ON (Along X-axis)\nProbability of finding the electron in Spin-Up |0>:\n\n";

	for(double i=0;i<=time;i+=dt) 
	{
		q.apply("rx",0,freq*dt);
		prob=q.prob(0);	//Prob of spin-up
		pos=static_cast<int>(prob*width);
		std::cout<<"t="<<std::fixed<<std::setprecision(2)<<i<<"s |";
		for(int j=0;j<=width;j++) 
		{
			if(j==pos)
				std::cout<<"O";
			else if(j<pos)
				std::cout<<"=";
			else 
				std::cout<<" ";
		}
		std::cout<<"| "<<static_cast<int>(prob*100)<<"%\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	return 0;
}
