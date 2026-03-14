CXX = g++

# IMPORTANT: Change this path to wherever Eigen is installed on your system!
# EG: Common Linux path is /usr/include/eigen3
EIGEN_PATH = /usr/include/eigen3
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -I $(EIGEN_PATH)

EXEC_SHELL = quantum_shell
EXEC_RABI = rabi_graph
EXEC_LARMOR = bloch_tracker

all: shell rabi tracker

shell: quantum_shell.cpp Quantum.hpp Kronecker.hpp
	$(CXX) $(CXXFLAGS) quantum_shell.cpp -o $(EXEC_SHELL)

rabi: rabi.cpp Quantum.hpp Kronecker.hpp
	$(CXX) $(CXXFLAGS) rabi.cpp -o $(EXEC_RABI)

tracker: larmor.cpp Quantum.hpp Kronecker.hpp
	$(CXX) $(CXXFLAGS) larmor.cpp -o $(EXEC_LARMOR)

clean:
	rm -f $(EXEC_SHELL) $(EXEC_RABI) $(EXEC_LARMOR)
