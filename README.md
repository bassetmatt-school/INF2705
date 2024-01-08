# INF2705
Projects introducing OpenGL technologies.

## Requirements
- `make`
- `gcc`, `clang` or any C++ compiler
- `SDL2`
- `glew`
- `freeimage` (`freeimage` on arch based distros, `libfreeimage-dev` on Ubuntu based distros)

## Running
Go to the source folder and run
```bash
make run
```

or build it with `cmake` (implying you have `cmake`)
```bash
mkdir build && cd build
cmake .. && make
```

And then run it
```bash
cd ../bin
./tp3
```
Or build+run it directly via the VS Code extension if you have it.
