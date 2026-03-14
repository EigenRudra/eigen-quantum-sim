# Eigen Quantum Simulator ⚛️

A high-performance, light-weight Quantum Physics Simulator written in C++. 

This project goes beyond standard discrete-gate circuit building. It features a modular backend powered by the **Eigen C++ template library** for highly optimized matrix operations, custom tensor (Kronecker) products to mathematically expand standard $2\times2$ quantum gates into the full $2^n\times2^n$ Hilbert space, and a continuous time-evolution engine for simulating real-world quantum dynamics like Larmor Precession.

*PS: No AI tools have been used in generating the code for the core logic of the project :)*

## 🚀 Features & Included Tools
This repository contains a shared physics engine powering three distinct simulation frontends:
1. **Interactive Circuit Shell:** Build custom quantum circuits, entangle qubits, and collapse wavefunctions via a terminal CLI.
2. **1D Time-Evolution (Rabi Oscillations):** A terminal-based visualizer showing the live probability shifting of a single qubit over time under a magnetic field.
3. **3D Bloch Sphere Tracker:** Calculates exact $\langle X \rangle, \langle Y \rangle, \langle Z \rangle$ expectation values and pipes the telemetry to a Python Plotly script for interactive 3D rendering.

---

## 🧠 The Physics Behind-the-Scenes

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

---

## 📁 Project Structure
**The Engine (Backend)**
* `Quantum.hpp`: A modular, header-only library containing the core `Quantum` engine class.
* `Kronecker.hpp`: Contains the custom, vectorized Kronecker product implementation for Eigen sparse matrices.

**The Applications (Frontends)**
* `quantum_shell.cpp`: The interactive discrete-gate circuit builder.
* `rabi.cpp`: Real-time ASCII visualization of Rabi oscillations.
* `larmor.cpp`: 3D telemetry tracker calculating continuous expectation values.
* `visualize.py`: Python script for interactive 3D Bloch sphere rendering.
* `Makefile`: Automates the C++ compilation process.

---

## ⚙️ Dependencies
**C++ Core:**
* C++17 Compiler (GCC, Clang, or MSVC)
* [Eigen 3](https://libeigen.gitlab.io/): C++ template library for linear algebra.

**Python 3D Visualization (Optional):**
* Python 3.x
* Libraries: `pandas`, `plotly`, `numpy`

---

## 🛠️ How to Build and Run

1. Ensure Eigen is installed on your system.
2. Edit the `EIGEN_PATH` variable in the `Makefile` to point to your local Eigen directory (e.g., `/usr/include/eigen3`).
3. Open your terminal and build the entire suite:
```bash
make
