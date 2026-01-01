# Quick Start Guide

## Build

```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get install libsdl2-dev libglew-dev libgl1-mesa-dev

# Compile ultra-compressed version (43 lines)
gcc -O3 -std=c99 -Wall -Wno-unused-function \
  -I/usr/include/SDL2 -D_REENTRANT \
  q2_ultra.c -o q2_ultra \
  -lSDL2 -lGLEW -lGL -lm

# Or compile original version (55 lines)
gcc -O3 -std=c99 -Wall -Wno-unused-function \
  -I/usr/include/SDL2 -D_REENTRANT \
  q2.c -o q2 \
  -lSDL2 -lGLEW -lGL -lopenal -lm
```

## Extract Assets

You need Quake 2 assets in `baseq2/` directory:

```bash
# If you have pak0.pak
mkdir baseq2
./unpak pak0.pak  # extracts to current directory
mv textures models pics maps baseq2/

# Or copy from installed Quake 2
cp -r /path/to/quake2/baseq2 .
```

## Run

```bash
# Run with default map
./q2_ultra

# Run with specific map
./q2_ultra baseq2/maps/demo1.bsp
./q2_ultra baseq2/maps/demo2.bsp
./q2_ultra baseq2/maps/demo3.bsp
```

## Controls

- **W/A/S/D** - Move forward/left/backward/right
- **Mouse** - Look around (captured)
- **ESC** - Quit

## Features

The ultra-compressed version (`q2_ultra.c` - 43 lines) includes:
- BSP map rendering
- Texture mapping
- First-person camera
- Mouse look
- Movement physics
- Screenshot capture (auto-saved as `ultra_*.ppm`)

The original version (`q2.c` - 55 lines) adds:
- MD2 model loading and rendering
- Lightmaps
- Enemy AI
- Projectile system
- OpenAL audio

## File Structure

```
QuakeII/
├── q2_ultra.c          # Ultra version (43 lines)
├── vert_ultra.glsl     # Ultra vertex shader (8 lines)
├── frag_ultra.glsl     # Ultra fragment shader (6 lines)
├── q2.c                # Original version (55 lines)
├── vert.glsl           # Original vertex shader
├── frag.glsl           # Original fragment shader
└── baseq2/             # Game assets
    ├── maps/           # BSP maps
    ├── textures/       # WAL textures
    ├── models/         # MD2 models
    └── pics/           # PCX images
```

## Troubleshooting

**Black screen?**
- Make sure baseq2/maps/*.bsp exists
- Check baseq2/textures/ has .wal files
- Verify baseq2/pics/colormap.pcx exists

**Can't compile?**
- Install SDL2, GLEW, OpenGL dev packages
- Check pkg-config works: `pkg-config --libs sdl2`

**Slow performance?**
- Use `-O3` optimization flag
- Check OpenGL version: `glxinfo | grep OpenGL`
- Try software rendering if GPU drivers missing
