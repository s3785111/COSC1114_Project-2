# COSC1114 Operating Systems Principals Assignment 1

 - Author: Matthew Ricci
 - Student Number: s3785111

#### Table of Contents
<!-- mtoc-start -->
1. [Build Instructions](#build-instructions)
2. [Executing the binaries](#executing-the-binaries)
3. [Example execution workflow](#example-execution-workflow)
<!-- mtoc-end -->

Locks and conditions are implemented in ```/mscopier/src/mscopier.c``` within thread functions ```mscopier_read``` and ```mscopier_write``` on lines 97 and 142 respectively.

### Build Instructions
To build all components of the project, navigate to the root directory (A1/) and run one of the following commands:

```bash
make                   # Standard build
make "CFLAGS=-Werror"  # Build with -Werror
make "CFLAGS=-D DEBUG" # Build with debug output
```

Subprojects may be individually built by navigating to their respective directories (mmcopier/ or mscopier/) and invoking ```make```.

### Executing the binaries

> [!IMPORTANT]
> For either binary, the input and output files/directories *must* be provided as relative to the working directory from which you invoke it. For example, if you choose to invoke ```mscopier``` from the root directory with ```./bin/mscopier```, then the input files/directories must be relative to the root directory.

After building, the mmcopier executable will be located in the mmcopier/bin directory. To run it, use:

```
./mmcopier/bin/mmcopier [thread count] [input dir] [output dir]
```

Similarly, after building, the mscopier executable will be located in the mscopier/bin directory. To run it, use:

```
./mscopier/bin/mscopier [thread "team" count] [input file] [output file]
```

Both projects additionally have their binaries symbolically linked within the top level ```/bin``` located in root for convenience.

---

### Example execution workflow
> [!IMPORTANT]
> In order to successfully run the built binaries it is important that the input and output directories (in the case of ```mmcopier```) and the input file (in the case of ```mscopier```) already exist prior to invoking their executables.
> 
Assume the project structure provided below:
```
├── bin
├── makefile
├── mmcopier
│   ├── bin
│   ├── build
│   ├── makefile
|   ├── input_dir
│   │   ├── file1.txt
│   │   ├── file2.txt
│   │   └── file3.txt
|   ├── output_dir
│   └── src
│       ├── mmcopier.c
│       ├── mmcopier.h
│       ├── pathcat.c
│       └── pathcat.h
├── mscopier
│   ├── bin
│   ├── build
│   ├── makefile
│   ├── infile.txt
│   └── src
│       ├── mscopier.c
│       ├── mscopier.h
│       ├── pathcat.c
│       └── pathcat.h
└── README.md
```

The workflow for building and executing both projects would look as follows:

1. Build both subprojects from within the root directory by invoking ```make "CFLAGS=-Werror"```
2. Run ```mmcopier``` from within the root directory:
```bash
./bin/mmcopier 3 mmcopier/input_dir mmcopier/output_dir
```
3. Run ```mscopier``` from within the root directory:
```bash
./bin/mscopier 10 mscopier/infile.txt mscopier/outfile.txt
```

---
