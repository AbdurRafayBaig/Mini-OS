# Mini-Operating-System

**OS-Simulation-System** <br>
This project is a C++-based Operating System Simulation System, developed inside a Linux environment using VMware. It delivers a simplified yet practical demonstration of how core OS functionalities work, featuring both Kernel Mode and User Mode logic, a graphical interface, and live app execution — all built from scratch.

**Core Features:** <br>
**Graphical Desktop Interface** <br>
A fully interactive GUI shell that simulates a desktop, allowing the user to access and manage multiple in-built apps from a central location.

**Kernel and User Mode Separation** <br>
The system emulates the structural flow between kernel-level operations (such as control and coordination) and user-level utilities, giving clear separation of roles — just like in real OS designs.

**Startup Resource Allocation** <br>
On boot, the system prompts users to input RAM size and number of GPU cores, offering a hands-on simulation of system resource declaration and allocation.

**Real-Time Clock App** <br>
A live-running digital clock embedded within the OS, demonstrating constant background process execution.

**Basic Calculator** <br>
A simple arithmetic calculator that performs essential math operations within the GUI, integrated like a system utility.

**Notes Application** <br>
A lightweight notepad app allowing users to write, edit, and store quick notes — simulating personal file interaction.

All components are coordinated via main.cpp, which functions as a simulated OS kernel, handling transitions, execution flows, and mode changes. The directory structure also mimics a basic bootloader environment, including mock boot files to create a full-stack OS experience.

Built entirely in Linux and executed via VMware, this project serves as a powerful hands-on approach to understand process management, boot sequences, GUI handling, and low-level OS architecture through modern C++.
