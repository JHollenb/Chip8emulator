# Chip8emulator

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

# 8080 Emulator Tutorial
## Disassembly Process
Every processor has a reference manual made by the manufacturer which is called something along the lines of "Programmer's Environment Manual". These data sheet contain a mapping of opcodes to instructions. 

So how are these opcodes useful? Well, ROMs are comprised of opcodes. If we look at the hexdump of a ROM, you'll see the opcodes printed out: `hexdump -v someRom.h`

Note that not all instructions are one byte. Some are two, some are three. The purpose of a disassembler is to take this hexdump and turn it into human readable assembly code.

## Memory Maps
TODO - Why is this important to us?

For the 8080, the map is as follows:
```
   Memory map:    
    ROM    
    $0000-$07ff:    invaders.h    
    $0800-$0fff:    invaders.g    
    $1000-$17ff:    invaders.f    
    $1800-$1fff:    invaders.e    

    RAM    
    $2000-$23ff:    work RAM    
    $2400-$3fff:    video RAM    

    $4000-:     RAM mirror 
```

If the map isn't document, you can find it a few different ways:
* GDB and make notes of the addresses that are being used
* Get the game's schematics and trace signals from the CPU's address pins. 
