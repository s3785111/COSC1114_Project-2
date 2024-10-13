# COSC1114 Operating Systems Principals Assignment 3

 - Author: Matthew Ricci
 - Student Number: s3785111

#### Table of Contents

1. [Build Instructions](#build-instructions)
2. [Executing the binaries](#executing-the-binaries)

### Build Instructions
To build all components of the project, navigate to the assignment root directory and run one of the following commands:

```bash
make                   # Standard build
make "NOWERROR=true"   # Build without -Werror
make "CFLAGS=-D DEBUG" # Build with debug output
```

This builds the allocator library as well as both subproject (bestfit and firstfit).

### Executing the binaries

> [!IMPORTANT]
> For either binary, the input file *must* be provided as relative to the working directory from which you invoke it. For example, if you choose to invoke ```firstfit``` from the root directory with ```./bin/firsfit```, then the input file must be relative to the root directory.

After building, the firstfit executable will be located in the firstfit/bin directory. To run it, use:

```
./firstfit/bin/firstfit [datafile]
```

Similarly, after building, the firstfit executable will be located in the firstfit/bin directory. To run it, use:

```
./firstfit/bin/firstfit [datafile]
```

Both projects additionally have their binaries symbolically linked within the top level ```/bin``` located in root for convenience.

---
