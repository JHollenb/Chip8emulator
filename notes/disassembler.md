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

