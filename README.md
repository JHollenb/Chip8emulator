# Chip8emulator
## Plan of Attack

1. Build disassembler
1. Build emulator
1. Incorporate hardware drivers

Most of this is pretty cookie cutter from the 8080 tutorial.

## Overview
Create a cpu emulator for the Chip-8 processor. The Chip-8 is a processor from the 70's that is an excellent introduction to hardware emulation as it has a limited instruction set and is very well documented. Writing the emulator from scratch will give you some very good exposure to the type of software we deal with. Here is a decent sequence of events:

* Research Chip8; gather data sheets / instruction set info; gather some sample Chip8 ROMs
* Create a disassembler and make sure it produces sensible output for a known good ROM
* Emulate the CPU (i.e. handle sequences of opcodes with your virtual processor)
* Emulate the peripheral devices like user input, sound / video output (This part is less important, so feel free to cheat on this part from existing code on the internet)

## Requirements:
* The emulator shall be able to disassemble a Chip-8 ROM and produce source assembly listings.
* The emulator shall be capable of executing a Chip-8 ROM without crashing.
* The emulator shall be able to accept user input via keyboard and produce a graphical display. 


## References & Resources
* [Emulator tutorial](http://emulator101.com)
