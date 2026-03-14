#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <cmath>
#include <fstream>
#include "Quantum.hpp"

int main() 
{
	double Bx,By,Bz,time,dt=0.05,ex,ey,ez;
	std::cout<<"Enter the Magnetic Field Vector (Larmor Frequencies):\n";
	std::cout<<"  Enter Bx: ";	std::cin>>Bx;
	std::cout<<"  Enter By: ";	std::cin>>By;
	std::cout<<"  Enter Bz: ";	std::cin>>Bz;
	std::cout<<"Enter Total Simulation Time: ";
	std::cin>>time;
	
	std::cout<<"\nMagnetic Field turned ON (Along ["<<Bx<<", "<<By<<", "<<Bz<<"]\n\n";
	Quantum q(1); 
	std::ofstream file("trajectory.csv");
	if (!file.is_open()) 
	{
		std::cout<<"Could not open trajectory.csv for writing!\n";
		return 1;
	}
	file<<"t,x,y,z\n";
	std::cout << std::left << std::setw(15) << "Time(in s)" << std::right << std::setw(12) << "<X>" << std::setw(12) << "<Y>" << std::setw(12) << "<Z>\n";
    	std::cout<<std::string(80,'-')<<"\n";
    	
	for(double i=0;i<=time;i+=dt) 
	{
		ex=q.expect("x",0);
		ey=q.expect("y",0);
		ez=q.expect("z",0);
		if(std::abs(ex)<1e-10)	//To clean floating point data if any due to rounding errors
			ex=0;
		if(std::abs(ey)<1e-10)
			ey=0;
		if(std::abs(ez)<1e-10)
			ez=0;
		file<<i<<","<<ex<<","<<ey<<","<<ez<<"\n";
		std::cout << "t=" << std::fixed << std::setprecision(2) << std::left << std::setw(13) << i << std::right << std::setprecision(4) << std::setw(12) << ex << std::setw(12) << ey << std::setw(12) << ez << "\n";
		
		if(Bx!=0)
			q.apply("rx",0,Bx*dt);
		if(By!=0)
			q.apply("ry",0,By*dt);
		if(Bz!=0)
			q.apply("rz",0,Bz*dt);
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	file.close();
	std::cout<<"\nSimulation Completed! Data exported to 'trajectory.csv'\n";
	std::cout<<"Run 'python3 visualize.py' to view the 3D Bloch Sphere trajectory\n";
	return 0;
}
