# Eigen Quantum Simulator

A high-performance, light-weight interactive Quantum Physics Simulator written in C++. 

It allows users to construct quantum circuits, apply standard gates, entangle qubits and simulate wavefunction collapse via an interactive terminal shell. This simulator leverages the **Eigen C++ template library** for highly optimized matrix operations and uses custom tensor (Kronecker) products to mathematically expand standard 2x2 quantum gates into the full $2^n\times2^n$ Hilbert space.

**UPDATED:**
* The code has now been converted from dense matrices to sparse matrices to allow computation of up to 26-qubit systems from previously 14-qubit system.
* This project now goes beyond standard discrete-gate circuit building. Utilising our custom modular backend, the project now also includes a continuous time-evolution engine for simulating real-world quantum dynamics like Larmor Precession.

*PS: No AI tools have been used in generating the code for the core logic of the project :)*

## Features & Included Tools
This repository contains a shared physics engine powering three distinct simulation frontends:
1. **Interactive Circuit Shell:** Build custom quantum circuits, entangle qubits, and collapse wavefunctions via a terminal CLI.
2. **1D Time-Evolution (Rabi Oscillations):** A terminal-based visualizer showing the live probability shifting of a single qubit over time under a magnetic field.
3. **3D Bloch Sphere Tracker:** Calculates exact $\langle X \rangle, \langle Y \rangle, \langle Z \rangle$ expectation values and pipes the telemetry to a Python Plotly script for interactive 3D rendering.

## The Physics behind-the-scenes

This simulator models quantum systems using the **State-Vector** approach. 

### The State Vector
An $n$-qubit system is represented by a complex vector of size $2^n$ in a Hilbert space. For a single qubit in superposition, the state $|\psi\rangle$ is defined as:
$$|\psi\rangle=\alpha|0\rangle+\beta|1\rangle$$
Where $\alpha$ and $\beta$ are complex probability amplitudes, and $|\alpha|^2+|\beta|^2=1$.

### Gate Operations & The Kronecker Product
Quantum gates are unitary matrices. A single-qubit gate (like the $2\times2$ Hadamard matrix) applied to a specific target qubit in an $n$-qubit system must be expanded to a $2^n\times2^n$ matrix. This is achieved using the Kronecker (tensor) product $\otimes$. 

For example, applying a gate $U$ to the middle qubit in a 3-qubit system translates to:
$$U_{total}=I\otimes U\otimes I$$
The `userdefined::Kronecker` function in this engine efficiently computes these tensor products to scale $2\times2$ gates up to the full system dimension.

### Continuous Time Evolution
Instead of just applying instantaneous logic gates, the simulator can solve the time-dependent Schrödinger equation:
$$U(t) = e^{-i \frac{H t}{\hbar}}$$
By applying a magnetic field vector $\vec{B} = (B_x, B_y, B_z)$ over tiny time steps ($dt$), the engine simulates the physical precession of an electron's magnetic moment (Larmor Precession).

### Expectation Values & Measurement
To extract 3D Cartesian coordinates for the Bloch sphere visualization, the engine calculates the expectation value of the Pauli matrices:
$$\langle X \rangle = \langle \psi | \sigma_x | \psi \rangle$$
When the `measure` command is called, the wavefunction permanently collapses. The probability $P$ of the system collapsing into a specific basis state $|x\rangle$ is given by the Born rule:
$$P(x)=|\langle x|\psi\rangle|^2$$

## Project Structure
**The Engine (Backend)**
* `Quantum.hpp`: A modular, header-only library containing the core `Quantum` engine class.
* `Kronecker.hpp`: Contains the custom, vectorized Kronecker product implementation for Eigen sparse matrices.

**The Applications (Frontends)**
* `quantum_shell.cpp`: The interactive discrete-gate circuit builder.
* `rabi.cpp`: Real-time ASCII visualization of Rabi oscillations.
* `larmor.cpp`: 3D telemetry tracker calculating continuous expectation values.
* `visualize.py`: Python script for interactive 3D Bloch sphere rendering.
* `Makefile`: Automates the C++ compilation process.

## Dependencies
**C++ Core:**
* C++17 Compiler (GCC, Clang, or MSVC)
* [Eigen 3](https://libeigen.gitlab.io/): C++ template library for linear algebra.

**Python 3D Visualization:**
* Python 3.x
* Libraries: `pandas`, `plotly`, `numpy`


## How to Build and Run

1. Ensure Eigen is installed on your system.
2. Edit the `EIGEN_PATH` variable in the `Makefile` to point to your local Eigen directory.
3. Open your terminal and build the entire suite:

```bash
make
```

This will generate three executables: `./quantum_shell`, `./rabi_graph`, and `./bloch_tracker`.
___

## Tool 1: Interactive Circuit Builder (`./quantum_shell`)
Launch the interactive CLI to build static quantum circuits.

**Available Commands:**
* `h <target>` : Hadamard Gate
* `x <target>` : Pauli-X (NOT) Gate
* `y <target>` : Pauli-Y Gate
* `z <target>` : Pauli-Z Gate
* `s <target>` : Phase (S) Gate
* `t <target>` : Pi/8 (T) Gate
* `rx <target> <theta>` : Parameterized X-Rotation
* `ry <target> <theta>` : Parameterized Y-Rotation
* `rz <target> <theta>` : Parameterized Z-Rotation
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

![output1](https://github.com/user-attachments/assets/be6f08c6-3f06-4b2f-b197-6d9efc06f000)

### Example 2: Creating a GHZ State (3-Qubit Entanglement)
```text
Enter no.of qubits (1-30): 3
QuantumShell> h 0
QuantumShell> cnot 0 1
QuantumShell> cnot 1 2
QuantumShell> state
QuantumShell> measure
```
![output2](https://github.com/user-attachments/assets/3c304f62-0200-4189-8c63-59fce726b45a)

### 5-Qubit Uniform Superposition
```text
Enter no.of qubits (1-30): 5
QuantumShell> h 0
QuantumShell> h 1
QuantumShell> h 2
QuantumShell> h 3
QuantumShell> h 4
QuantumShell> state
QuantumShell> measure
```
![output3](https://github.com/user-attachments/assets/dc3f2207-0bec-4102-8515-c4431eb21d18)

### Phase Kickback and Negative Interference
```text
Enter no.of qubits (1-30): 2
QuantumShell> x 1
QuantumShell> h 0
QuantumShell> h 1
QuantumShell> cnot 0 1
QuantumShell> h 0
QuantumShell> measure
```

![output4](https://github.com/user-attachments/assets/89bac087-f0dc-4cb2-b8de-e41f814c425a)

___
## Tool 2: 1D Time Evolution (`./rabi_graph`)
Simulate a single qubit in a magnetic field. This generates a live terminal animation showing the probability of the electron being in the Spin-Up |0> state oscillating over time.
```bash
./rabi_graph
```
![output5](https://github.com/user-attachments/assets/b8392e25-df60-4dcb-9e55-64604b1d362d)

## Tool 3: 3D Bloch Sphere Dynamics (`./bloch_tracker`)
Simulate continuous Larmor precession and visualize it in 3D. 

1. Run the tracker and define your 3D magnetic field vector (Bx, By, Bz).
```bash
./bloch_tracker
```

2. The C++ engine will print the live telemetry to the terminal and export the exact coordinates to `trajectory.csv`.

3. Render the interactive 3D plot using Python:

```bash
# Creating a Python virtual environment first
python3 -m venv venv
source venv/bin/activate
pip install pandas plotly numpy
python3 visualize.py
```
(This will open a browser window displaying the interactive Bloch Sphere and the state vector trajectory).

![output6](https://github.com/user-attachments/assets/d713e481-20da-4fcc-a35c-d80ab4ee0a56)
