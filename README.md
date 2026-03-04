# Eigen Quantum Simulator

A light-weight, interactive state-vector quantum circuit simulator written in C++. It allows users to construct quantum circuits, apply standard gates, entangle qubits and simulate wavefunction collapse via an interactive terminal shell.

This simulator leverages the **Eigen C++ template library** for highly optimized matrix operations and uses custom tensor (Kronecker) products to mathematically expand standard 2x2 quantum gates into the full $2^n\times2^n$ Hilbert space.

PS: No AI tools has been used in generating the code for the core logic of the project :)

## The Physics Under the Hood

This simulator models quantum systems using the **State-Vector** approach. 

### The State Vector
An $n$-qubit system is represented by a complex vector of size $2^n$ in a Hilbert space. For a single qubit in superposition, the state $|\psi\rangle$ is defined as:
$$|\psi\rangle=\alpha|0\rangle+\beta|1\rangle$$
Where $\alpha$ and $\beta$ are complex probability amplitudes, and $|\alpha|^2+|\beta|^2=1$.

### Gate Operations & The Kronecker Product
Quantum gates are unitary matrices. A single-qubit gate (like the 2x2 Hadamard matrix) applied to a specific target qubit in an $n$-qubit system must be expanded to a $2^n\times2^n$ matrix. This is achieved using the Kronecker (tensor) product $\otimes$. 

For example, applying a gate $U$ to the middle qubit in a 3-qubit system translates to:
$$U_{total}=I\otimes U\otimes I$$
The `userdefined::Kronecker` function in this engine efficiently computes these tensor products to scale 2x2 gates up to the full system dimension.

### Measurement
When the `measure` command is called, the wavefunction collapses. The probability $P$ of the system collapsing into a specific basis state $|x\rangle$ is given by the Born rule:
$$P(x)=|\langle x|\psi\rangle|^2$$
The simulator calculates this squared norm for every possible state, generates a probability distribution, and uses a random number generator to select the final collapsed state.

## Files
* `main.cpp`: Contains the core `Quantum` engine class and the interactive CLI loop.
* `Kronecker.hpp`: Contains the custom, vectorized Kronecker product implementation for Eigen matrices.
* `Makefile`: Automates the compilation process.

## Dependencies
* C++17 Compiler (GCC, Clang, or MSVC)
* [Eigen 3](https://libeigen.gitlab.io/): A C++ template library for linear algebra.

## How to Build and Run
1. Ensure Eigen is installed on your system.
2. Edit the `EIGEN_PATH` variable in the `Makefile` to point to your local Eigen directory.
3. Open your terminal and run:

```bash
make
./quantum_sim
```

## Available Commands
Inside the `QuantumShell>`, you can type:

* `h <target>` : Hadamard Gate
* `x <target>` : Pauli-X (NOT) Gate
* `y <target>` : Pauli-Y Gate
* `z <target>` : Pauli-Z Gate
* `s <target>` : Phase (S) Gate
* `t <target>` : Pi/8 (T) Gate
* `cnot <control> <target>` : CNOT (Entanglement) Gate
* `state` : Print current complex state vector
* `measure` : Collapse the wavefunction and show probability histogram
* `exit` : Quit simulator

### Example 1: Creating a Bell State

```text
Enter no.of qubits (1-10): 2
QuantumShell> h 0
QuantumShell> cnot 0 1
QuantumShell> state
QuantumShell> measure
```

### Example 2: Creating a GHZ State (3-Qubit Entanglement)
```text
Enter no.of qubits (1-10): 3
QuantumShell> h 0
QuantumShell> cnot 0 1
QuantumShell> cnot 1 2
QuantumShell> state
QuantumShell> measure
```

