# Assembler assignment

The assignmet is creating assembler program to an imaginary computer (for exemple 14-bit memory word length).
The program input is assembly files, and it's output is "encrypted" 14-bits memory image in case of success (and some more file for a linker), or print (to stdout) assembly errors.

The assembler mustn't stop it's execution under no circumstances, and creats all the necessary output files. 

## Further explantion

The input to the Assembler is(are) a file(n) name in the project directory, without any ending. The Assembler has three pass over the source code three times and creates the necessary output files.
In any time the Assembler finds an error in the current working file, it reports aboute it, don't continue 

### PreAssembly pass
The first time the Assembler pass the assembly code "PreAssembly", it creates an "extension" source code file. It deletes comments, empty rows, and macros delarations, and extendes the macros.

### First pass
In the "First pass", the Assembler interprest and validates most of the extends source code(and saves spaces holders for unknown yet date). It creates the memory image, saves most of the data, and saves the necessary labels (rellevant to the assembly syntax and content for the next pass).

### Second pass 
In the second pass the Assembly interprets the, now-known-labels, saves it to the the data to the memory image, and "encrypte" the data. At the end it creates the output files and save them in the project directiory.
