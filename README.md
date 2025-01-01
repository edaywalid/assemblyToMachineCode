# Assembly to machine code Converter 

This program converts assembly instructions (MOV until now) into their binary representation in hexadecimal format. It is a simple and efficient tool designed for developers working with assembly language and low-level programming.

## Usage

To use the program, run the following command:

```bash
./main -i "INSTRUCTION"
```

### Example

```bash
./main -i "MOV AX, BX"
```

The program will output the binary representation of the given MOV instruction in hexadecimal format, such as:

```bash
8BC3
```

## Features

- Converts MOV instructions into their binary representation.
- Supports various MOV instruction formats and register combinations.
- Easy-to-use command-line interface.
- Lightweight and fast execution.

## Installation

### Prerequisites

- A C compiler, such as GCC.
- A Linux-based system (or WSL for Windows users).
- make

### Steps

1. Clone the repository:
   ```bash
   git clone https://github.com/edaywalid/assemblyToMachineCode
   ```

2. Navigate to the project directory:
   ```bash
   cd assemblyToMachineCode
   ```

3. Compile the program:
   ```bash
   make
   ```

## Command-Line Options

- `-i "INSTRUCTION"`: Specify the MOV instruction to convert. Make sure to use quotes around the instruction to prevent shell interpretation errors.

## Supported Instructions


The program supports a wide range of MOV instructions. Below are the supported operation types:

- regToReg: Moves data between two general-purpose registers (e.g., MOV AX, BX).

- regToMem: Moves data from a general-purpose register to memory (e.g., MOV [AX], BX).

- memToReg: Moves data from memory to a general-purpose register (e.g., MOV BX, [AX]).
 
- ImmToRegM: Moves an immediate value to a memory location (e.g., MOV [AX], 0x1234).

- ImmToReg: Moves an immediate value to a register (e.g., MOV AX, 0x1234).

- MemToAcc: Moves data from a memory address to the accumulator register (e.g., MOV AL, [0x1234]).

- AccToMem: Moves data from the accumulator register to a memory address (e.g., MOV [0x1234], AL).

- rmToSreg: Moves data from a register/memory location to a segment register (e.g., MOV DS, AX).

- SregTorm: Moves data from a segment register to a register/memory location (e.g., MOV AX, DS).

Note: Currently, the program only supports x86-64 assembly syntax.

## Requirements

- A C compiler (e.g., GCC or Clang).
- Compatible with Linux-based systems. 

### Optional

- Windows users can use WSL (Windows Subsystem for Linux) or MinGW to run the program.

## Testing

After compilation, test the program with various MOV instructions to ensure accuracy. Example:

```bash
./main -i "MOV AX, BX"
./main -i "MOV [AX], RCX"
./main -i "MOV DX, 1234"
```

If you encounter any issues, please report them in the Issues section of the repository.

---

For any issues, feature requests, or feedback, feel free to open an issue in the repository or contact the maintainer. We appreciate your input to make this tool better!
