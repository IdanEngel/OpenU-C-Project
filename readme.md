# Assembly Project - README

## About
This project implements a two-pass assembler in ANSI C90.  
It processes `.as` assembly source files and produces output files:
- `.am` — Preprocessed file (after macro expansion)
- `.ob` — Object file (binary machine code)
- `.ent` — Entry labels file
- `.ext` — External labels file

---

## How to Compile
Open the terminal inside the project directory and run:

```bash
make
```
This will compile all necessary `.c` files into an executable called `assembler`.

---

## How to Run
To process one or more `.as` files, use:

```bash
./assembler filename_01.as filename_02.as filename_N.as

```

Example:

```bash
./assembler test_01.as test_02.as 

```

The assembler will:
- Expand macros.
- Run **first pass** to build symbol and code tables.
- Run **second pass** to resolve addresses and create final binaries.
- Create output files **only if no errors are detected**.

All output files (`.am`, `.ob`, `.ent`, `.ext`) will be saved in the project directory.

The (`.ob`, `.ent`, `.ext`) files will be saved at the Output_Files directory.

---

## Output Files
For each `.as` input file:
- `filename.am` — Macro expanded source.
- `filename.ob` — Object code.
- `filename.ent` — Entry labels and their addresses (if any).
- `filename.ext` — External labels and usage locations (if any).

---

## Error Handling
- If any error is detected during **macro expansion**, **first pass**, or **second pass**, **no output files** will be generated for that file.
- Errors are printed to the terminal along with filename and line number.

---

## Clean Build
To remove all generated files (`*.o`, `*.am`, `*.ob`, `*.ext`, `*.ent`, and the executable):

```bash
make clean
```

---

## Requirements
- Compiler: **gcc**
- System: Linux or any Unix-based environment
- C standard: **ANSI C90**

---

# Thank You! 🚀
