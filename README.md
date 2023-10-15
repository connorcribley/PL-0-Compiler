# PL-0-Compiler
A compiler for the PL/0 programming language built using C

To compile & run this code, first download the pl-0-compiler.c file and any requisite
input files to the same directory.

This project includes a correct case input file (correctin.txt), a correct case output file
(correctout.txt), an output file with executable code for a PL/0 virtual machine (elf.txt)
and five different incorrect case input and output files (which are related to procedure and
call statements). These are the incorrect case input files and their corresponding errors:

errorin1.txt: Const, var, procedure must be followed by identifier.
errorin2.txt: Assignment to constant or procedure is not allowed
errorin3.txt: Call must be followed by an identifier
errorin4.txt: Call of a constant or variable is meaningless
errorin5.txt: Expression must not contain a procedure identifier.

Use "gcc -g pl-0-compiler.c" to generate a program file from the .c file.

You can now run the program while passing in your input file with this
command: "./a.out input_file.txt".

The expected output will be shown directly in the terminal. Likewise,
an output file with executable code for a PL/0 virtual machine (elf.txt)
will be created.
