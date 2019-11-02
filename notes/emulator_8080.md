# Emulator Overview

# Emulating Instructions
An emulator is responsible for converting a given instruction into a machine specific operation. For example, storing specific values in reserverd spaces (registers and memory).

It works as follows:

1. Receive instruction (opcode).
1. Decide update various global registers based on the given opcode.

The example I'm looking at is a giant case statement. So if your processor has 255 instructions, you'll need 255 cases. 

The 8080 has 7 different types of operations:
### Data Transfer
### Arithmetic
8080 uses *Condition Codes* or flags. These flags are as follows:
* Z - 1 when result is 0
* S - 1 when bit 7 of math instruction is set
* P - Set when answer has even parity
* CY - Set when instruction results in carry out or borrow

See [arithmetic examples](http://www.emulator101.com/arithmetic-group.html).

### Logical
See [logical examples](http://www.emulator101.com/logical-group.html)

### Branch
See [branch examples](http://www.emulator101.com/branch-group.html).

### Stack
See [stack examples](http://www.emulator101.com/stack-group.html).

### I/O
See [I/O & Special examples](http://www.emulator101.com/io-and-special-group.html)
