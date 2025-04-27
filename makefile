# Compilation settings
CC = gcc
CFLAGS = -ansi -Wall -pedantic -g
EXE_DEPS = assembler.o macro_expand.o first_pass.o utils.o instruction_tables.o codegen.o second_pass.o

# Executable
assembler: $(EXE_DEPS)
	$(CC) -g $(EXE_DEPS) $(CFLAGS) -o $@

# Object file rules
assembler.o: ./Source_Files/assembler.c
	$(CC) -c ./Source_Files/assembler.c $(CFLAGS) -o $@

utils.o: ./Source_Files/utils.c ./Header_Files/utils.h
	$(CC) -c ./Source_Files/utils.c $(CFLAGS) -o $@

instruction_tables.o: ./Source_Files/instruction_tables.c ./Header_Files/instruction_tables.h
	$(CC) -c ./Source_Files/instruction_tables.c $(CFLAGS) -o $@

macro_expand.o: ./Source_Files/macro_expand.c
	$(CC) -c ./Source_Files/macro_expand.c  $(CFLAGS) -o $@

first_pass.o: ./Source_Files/first_pass.c
	$(CC) -c ./Source_Files/first_pass.c $(CFLAGS) -o $@

codegen.o: ./Source_Files/codegen.c
	$(CC) -c ./Source_Files/codegen.c $(CFLAGS) -o $@

second_pass.o: ./Source_Files/second_pass.c
	$(CC) -c ./Source_Files/second_pass.c $(CFLAGS) -o $@


# Run assembler
run: assembler
	./assembler $(FILES)

# Clean temporary files
clean:
	rm -f *.o *.am Output_Files/*.ob Output_Files/*.ent Output_Files/*.ext assembler
