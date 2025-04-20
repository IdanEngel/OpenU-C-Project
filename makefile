##CC = gcc
##CFLAGS = -ansi -Wall -pedantic
##SRC = $(wildcard Source_Files/*.c)
#
##all: assembler
##
##assembler:
##	$(CC) $(CFLAGS) -IHeader_Files -o assembler $(SRC)
##
##clean:
##	rm -f assembler *.ob *.ent *.ext *.am
#
#
#CC = gcc
#CFLAGS = -ansi -pedantic -Wall -Wextra -std=c90
#
#SRC_DIR = Source_Files
#SRC = $(SRC_DIR)/macro_expand.c
#TARGET = macro_expand
#
#all: $(TARGET)
#
#$(TARGET): $(SRC)
#	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)
#
#run:
#	./$(TARGET) testprog.as test_output.am
#
#clean:
#	rm -f $(TARGET) *.o test_output.am


# Compilation settings
CC = gcc
CFLAGS = -ansi -Wall -pedantic -g
#GLOBAL_DEPS = globals.h
EXE_DEPS = assembler.o macro_expand.o first_pass.o utils.o instruction_tables.o
# Input/output files
INPUT_AS = testprog.as
OUTPUT_AM = test.am

# Executable
assembler: $(EXE_DEPS) $(GLOBAL_DEPS)
	$(CC) -g $(EXE_DEPS) $(CFLAGS) -o $@

# Object file rules
assembler.o: ./Source_Files/assembler.c $(GLOBAL_DEPS)
	$(CC) -c ./Source_Files/assembler.c $(CFLAGS) -o $@

utils.o: ./Source_Files/utils.c ./Header_Files/utils.h
	$(CC) -c ./Source_Files/utils.c $(CFLAGS) -o $@

instruction_tables.o: ./Source_Files/instruction_tables.c ./Header_Files/instruction_tables.h
	$(CC) -c ./Source_Files/instruction_tables.c $(CFLAGS) -o $@

macro_expand.o: ./Source_Files/macro_expand.c $(GLOBAL_DEPS)
	$(CC) -c ./Source_Files/macro_expand.c  $(CFLAGS) -o $@

first_pass.o: ./Source_Files/first_pass.c $(GLOBAL_DEPS)
	$(CC) -c ./Source_Files/first_pass.c $(CFLAGS) -o $@

#second_pass.o: ../SourceFiles/second_pass.c $(GLOBAL_DEPS)
#	$(CC) -c ../SourceFiles/second_pass.c $(CFLAGS) -o $@
#
#code_conversion.o: ../SourceFiles/code_conversion.c $(GLOBAL_DEPS)
	#$(CC) -c ../SourceFiles/code_conversion.c $(CFLAGS) -o $@

#data_strct.o: ../SourceFiles/data_strct.c $(GLOBAL_DEPS)
#	$(CC) -c ../SourceFiles/data_strct.c $(CFLAGS) -o $@
#
#table.o: ../SourceFiles/table.c $(GLOBAL_DEPS)
#	$(CC) -c ../SourceFiles/table.c $(CFLAGS) -o $@
#
#util.o: ../SourceFiles/util.c $(GLOBAL_DEPS)
#	$(CC) -c ../SourceFiles/util.c $(CFLAGS) -o $@
#
#Errors.o: ../SourceFiles/Errors.c $(GLOBAL_DEPS)
#	$(CC) -c ../SourceFiles/Errors.c $(CFLAGS) -o $@
#
#handle_text.o: ../SourceFiles/handle_text.c $(GLOBAL_DEPS)
#	$(CC) -c ../SourceFiles/handle_text.c $(CFLAGS) -o $@
#
#lexer.o: ../SourceFiles/lexer.c $(GLOBAL_DEPS)
#	$(CC) -c ../SourceFiles/lexer.c $(CFLAGS) -o $@

# Run assembler on input
run: assembler
	./assembler $(OUTPUT_AM) $(INPUT_AS)

# Clean temporary files
clean:
	rm -f *.o *.am *.ob *.ent *.ext assembler
