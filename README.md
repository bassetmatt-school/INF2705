# INF2705
Project to be introduced to OpenGL.

## Requirements
- `make`
- `gcc`, `clang` or any C++ compiler
- `SDL2`
- `glew`

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
Or run it directly via the VS Code extension if you have it.
## Usage
### Commands
- Space to lock/unlock the mouse.
- `T` to cycle through scenes.

### Description
Launch the programm, it will launch a scene containing a ball lit by 3 lights (move the mouse if the screen is blank, same goes for every scene).
Then you can cycle through scenes with `T`. There are 3 scenes:
- The lighting test scene (more on that later).
- The scene from TP2 (WASD to move, Shift/Mouse Button5 to sprint and Q/E to move vertically).
- The stencil test scene (Same controls). Used to test stencil test and blending. There are ally and enemy monkeys, the enemy monkeys have a red halo and the ally monkeys have a blue one that is visible through obstacles. There's a transparent glass in the middle of the scene that demonstrates blending.

### Lighting scene parameters
There are a whole lot of parameters you can play with in this scene. Here is a pretty exhaustive list:
- First you can select the model to display and apply light on. Choices: [Sphere, Cube, Monkey].
- The material properties or the main model where you can change:
  - Its color (emission, ambiant, diffuse, specular).
  - Its shininess (for specular reflexions).
- The light model:
  - You can select the method used to compute the lighting between 3 methods [Phong, Gouraud, Flat]. Phong does the main computation in the fragment shader, doing the interpolation regarding normals. Gouraud does it in the vertex shader, interpoling intensity. Flat is the Lambert lighting model, it computes the illumination for each face in a geometry shader.
  - Global ambiant: The light the scene is bathing in.
  - Bool checkboxes:
    - Blinn specular relfexion: Uses Blinn formula instead of Phong for specular relfexion (see more [here](https://learnopengl.com/Advanced-Lighting/Advanced-Lighting)).
    - Use diffuse and specular texture or not: Kind of straightforward.
    - Use spotlight: The light emitted by the spots are very concentrated like spotlights.
    - Use Direct3D model for spotlights: The border is smoother (see the function `computeSpot` in `phong.fs.glsl` to see the explicit formula difference).
  - Spot exponent: Used to compute the luminosity decrease when using spotlight.
  - Spot opening: The opening angle of the spot in degrees.
  - Lights menu:
    - For each light you can change its ambiant, diffuse and specular components, as well as the spot's position and orientation.
