# Notes for NI-REV Exam

## 1. Introduction to Reverse Engineering, Stack Frame Analysis

> **Dissassembling** - Transformation of executable code from machine code to assembler code of specific processor.
>
> **Decompilation** - Transformation of executable code from machine code to a higher programmnig language.

> **Canary** - In order to make stack smashing attacks harder. Canary is pushed on the stack between return value and the rest of the buffer in prologue. If an attacker is doing buffer overflow attack he needs to write over this canary. In epilogue the canary is checked and in case it doesn't match the previous canary, the program crashes. Canary is noticeble by **XOR** in epilogue. In prologue it's stored in [gs:0x14].
>
> **Security cookie (Windows)** - At the begging of the program a global variable security cookie is generated. In each function prologue this value is xored with a value inside EBP register.

>**Prologue** - Saving function return value, canary and shared registers if function is planning to use them
>
>**Epilogue** - Checking canary, restoring shared registers and returning to previous function

**Static analysis** - Analysis of code that is not running. It's not good when there are runtime dependent stuff, dynamic generated code, external dependent things.

**Name mangling** - Overloading of functions, operators and methods. Name is not enough, information about const,... , public, ..., arguments, ... are needed. 

**Variable padding** - Variable padding in classes is used to make program faster since in runtime program can jump by set number of bytes in class. Option **Packed** can be used to prevent this padding, since it's taking more space than neccessary.

**Application Binary Interface** - It's a convention that states how should binary code on target platform interact with other code and operating system.

## 2. Analysis of a Program’s Flow

**Basic Block** - It's a chunk of code which can be entered only in the first instruction and exited only by the last instruction.

**Control flow graphs (CFG)** - Multiple basic blocks connected with arrows. Arrows indicate where code can jump from one basic block to another.

**Entrance point** - Function main is not the entrance point. Address of a function point is stored in **AddressOfEntryPoint** in PE Header.

**_initterm and _initterm_e** - Function that have 2 pointers. To the start and end of the .rdata section. Goes through every function pointer in the array and executes it. __initterm_e does the same but on top handles exception possible caused by some of the executed function pointers.

**Import Address Table (IAT)** - Table with pointers to external functions (made by import and such). Can be modified and hacked

**Indirect jump** - Address where program should jump is unknow during compilation. The address is filled during runtime (IAT).

**Terminators** - __onexitbegin and __onexitend array. Global pointers are stored in there. When program ends by returning from main or other method such as calling exit() it goes throug this array and destroy global variables.

## 3. Analysis of C++ Classes

**Struct vs Class** - differentiate only with struct being defaultly public wheras class defaultly private.

**Eg.**

struct Simple {
    char f_char = 11;
    short f_short = 0x55AA;
    int f_int = 123456789;
}

In memory:  <br>
0x0012FEE0 11 00 AA 55 78 56 34 12 ...UxV4. <br>
0x0012FEE8 11 31 AA 55 78 56 34 12 .1.UxV4. <br>
0x0012FEF0 11 F3 AA 55 78 56 34 12 ...UxV4. <br>
0x0012FEF8 11 9A AA 55 78 56 34 12 ...UxV4.

Notice the padding in the second byte

**Inheretence** - ex. class A: public B, public C

A -> pVMT <br>
B -> B attributes <br>
C -> C attributes <br>
  -> A attributes

**Polymorphysm** - If struct or class have virtual methods, it has to have as well Virtual Method Table (VMT). VMT is being assigned in constructor to this->pVMT.

**VMT and RTTI**

![VMT and RTTI](/RTTI.png)

# 4. Disassembling and Obfuscation

**Linear walkthrough** - Byte by byte from begging to end in .text section. Can't differentiate code from data.

**Extended Linear Walkthrough** - Table reallocation. Using this table it's possible to differentiate table jumps (eg. switch statements) as data.

**Recursive walkthrough** - Follows the jump should be taken. All unvisited address is saved as data.

**Hybrid Walkthrough** - First using extended linear walktrough and then Recursive.

**Obfuscation** - Make disassembling, decompiling, debugging harder as well as harder for humans

**Obfuscation metrics**:
1. How much obfuscation bamboozles human (different types of metrics - length of program, O() notation times)
2. How much time it takes for automatic tool to get rid of obfuscation

**Obfuscation types**:
1. Layout obfuscation - Delete comments, variable and class names
2. Controlled obfuscations - unreachable if else, same if as else, ...

**Opaque Predicates and Variables** - Variables that are known to the programmer at the time of obfuscation, but for the deobfuscator are hard to guess.

**Death Code** - Adding irrelevant chunks of code. Sometimes behind Opaque Predicates and Variables.

**CFG Obfuscation** - After using **goto** in C/C++. Worse in Java for example because Java doesn't have a goto, but has goto in bytecode

**Removing library calls** - Essentialy coding own alternative to eg. linked list, memory allocation, STL, etc. Therefore map<> can become for_sure_not_map<>.

**Table interpretation** - Diving code into different segments and putting it into switch, goto, other. The switch is then run so it will go through every case, essentialy running the original code.

**Redundant arguments** - In functions or math expressions

**Paralelisation of code** - Very potent, very durable and very expensive

**Inlining** - Taking function body and putting it where was the function called. Reverse is called **Outlining**.

**Interleaving** - Putting 1 and more function to callee function with a switch that calls corresponding functions.

**Cloning** - Cloning methods

# 5. Compiler Stub Recognition

**Why to care about which compiler was used?** - Each compiler works differently, it can help us when analyzing the dissasembled code

**How to distinguish** - Different libraries imported. Decorated names start with different symbol for different compilers

**Identifying library** - If dynamically linked it's easy (CFF Explorer). Staticaly, then we can use string to find names of the functions. (IDA Pro - F.L.I.R.T.)

**Identifying library functions** - Functions signatures. Finding function based on their disassembled form.

# 6. Debugging and Anti-Debugging

**Debugger** - can read registers (CONTEXT). Either connect to an already started process or starts its own

**Software breakpoints** - Invoked by software. Stored in FS:\[0\] -> chain of exceptions. Can detect access to instructions. Changes memory

**Hardware breakpoints** - Limited amount of breakpoints (registers). Can detect access to instructions and memory. Doesn't change memory

**Tracing** - Tracing instructions means debugger traces executed instructions

**Debugging Kernel** - Eg. making kernel driver. Debugging kernel driver -> pc freezes. Need second computer to regulate the debugging.

**Anti-debugging** - Protection against debugging. It's an app that tries to make debugging harder or prevent it. 

**Ways to detect debugging**:
1. WinAPI has few function such as IsDebuggerPresent or CheckRemoteDebuggerPresent and more.
2. Process Heap. If process was created by debugger **ForceFlags** is set to zero.
3. Scanning process in the background
4. Detection by timing
5. Move important parts from main to __initterm
6. Run code before start. TLS callback (idk this one)
7. NTGlobalFlag


