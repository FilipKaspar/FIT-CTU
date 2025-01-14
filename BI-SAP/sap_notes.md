# <p style="text-align: center"><b>SAP</b></p>

### What is software

- **Firmware** - BIOS, address modes, ISA (Instruction Set Architecture) 
- **Operating system** - Privileges, structure of directories on disk, memory controllers, cores, managing proccess
- **Developers SW** - Assembler - code translater, simulator and debugger, libraries
- **Applications** - Programming languages, editors, web browsers, games

### What is hardware

- **Structure and architecture** - CPU, flow of data, how instructions are being executed
- **Memory hierarchy** - Cache, managing memory
- **User interface** - Display, Graphical interface, keyboard, mouse, ports
- **other interfaces** - DMA(Direct Memory Access), communication protocols

### Developing softwware

Being translated in 4 steps.

![translation](images/compiling.jpg)

On hardware level it's being translated as following

![translation_hw](images/translation_hw.jpg)

### Von Neumann architecture

**Parts**
- Main memory
- CPU:
    - Data part
        - ALU - Arrhytmic Logical Unit
        - Registers
    - Controlling part
        - Controller - Control unit
- I/O devices

![neumann_arch](images/von_neumann.jpg)

### Logical synthesys

Finding the best possible formule for certain problem

**Representation:**

1. Table
2. N-dimensional cube
3. Algebraicly
4. Maps

**Minimalization:**  

1. Karnaugh map (K-Map)
2. Other methods :)

**Realization** - On the level of logical gates

### Boolean Algebra Laws

![lwas](images/laws.jpg)

### Gates laws

![gates](images/gates.JPG)
![gates_1](images/gates_1.JPG)

**Example of Adding maching using logical gates**

![example](images/gates_ex.JPG)

### Karnaugh map examples

![k-example](images/4x4.JPG)
![2x2](images/2x2multi.JPG)

### Sequential synchronous circuit

Combination circuits - it's output is dirrectly dependent on it's input (adder)  
Finite-state machine - simple computer, can switch in multiple states  

**Process of making FSM - MOORE**  
The output registered with the state

![fsm_1](images/fsm_ex1.JPG)
![fsm_2](images/fsm_ex2.JPG)
![fsm_3](images/fsm_ex3.JPG)

**Process of making FSM - MEALY**  
The output is registered with the transfer from the state to different state (the arrow from state to state)

![fsm2_1](images/fsm1_ex1.JPG)
![fsm2_2](images/fsm1_ex2.JPG)

### Need to know how to add, convert, multiply, subtract binary numbers

**Adding obvious**  
Add them together

**Multiplying**  
![multi](images/multi.JPG)

**Subtraction**  
Ex. numbers 64 - 35.  
Step 1: 35 to binary  
Step 2: Change 0's and 1's  
Step 3: 64 to binary  
Step 4: Adding  
Step 5: **IF** there is a 1 on the most left position, add 1 to the result
![multi_ex](images/multi_ex.JPG)

### Doplňkový kód = (1st complement?)

![circle](images/circle.JPG)
![circle1](images/circle1.JPG)

**Know how full and half adder works**
![adder](images/adder.JPG)
![codes](images/kody.jpg)
