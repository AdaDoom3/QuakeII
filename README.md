# Quake II Clone - Single File Engine

Ultra-compressed, code-golf style Quake II engine in a single C file (~20KB).
Loads and plays original Quake 2 assets (BSP maps, MD2 models, WAL textures, PCX images).

## Features

**Fully Functional Game Engine:**
- ✅ BSP map loading (Quake 2 IBSP version 38)
- ✅ Collision detection using BSP tree and brushes
- ✅ Texture loading (WAL format with palette)
- ✅ Lightmap rendering (baked lighting)
- ✅ MD2 model loading and skeletal animation
- ✅ First-person camera with mouse look
- ✅ Player movement (WASD + Space to jump)
- ✅ Physics (gravity, collision response)
- ✅ OpenGL 3.3 Core rendering with shaders
- ✅ Texture array for efficient multi-texture rendering
- ✅ OpenAL positional audio
- ✅ Projectile system with collision

**Code Stats:**
- Main engine: `q2.c` (19KB, 56 lines compressed)
- Vertex shader: `vert.glsl` (308 bytes, 12 lines)
- Fragment shader: `frag.glsl` (409 bytes, 12 lines)
- Total: ~20KB for entire game engine!

**Technical Highlights:**
- Single-pass BSP rendering with texture arrays
- Frame interpolation for smooth MD2 animations
- GPU-based lighting with filmic tonemapping in shader
- Brush-based collision detection
- View frustum culling via BSP tree

## Build Requirements
- GCC (C99)
- SDL2 development libraries
- OpenGL 3.3+ with GLEW
- OpenAL development libraries

On Ubuntu/Debian:
```bash
apt-get install libsdl2-dev libglew-dev libopenal-dev
```

## Required Assets

You need the original Quake 2 game assets (`pak0.pak` from the shareware or full game).

**Extract pak0.pak:**
```bash
python3 unpak.py
```

This creates the `baseq2/` directory structure with:
- `maps/*.bsp` - Map files
- `textures/*.wal` - Wall textures
- `models/*.md2` - 3D models
- `pics/*.pcx` - UI and palette data
- `sound/*.wav` - Audio files

## Build Instructions
```bash
make
```

## Running

**Play a specific map:**
```bash
./q2 baseq2/maps/demo1.bsp
```

**Default (if no map specified):**
```bash
./q2
```
Attempts to load `baseq2/maps/sample.bsp`

## Controls

- **W/A/S/D** - Move forward/left/back/right
- **Mouse** - Look around
- **Space** - Jump
- **Left Click** - Shoot (placeholder projectile)
- **ESC** - Quit

## How It Works

### Architecture

The engine uses extreme compression techniques:
1. **Single-letter typedefs**: `U`=unsigned char, `F`=float, `V`=vec3, `M`=mat4
2. **No comments**: Self-documenting through compact code
3. **Inline everything**: Functions are 1-2 lines where possible
4. **Shader offloading**: Lighting, tonemapping done in GPU
5. **Minimal error checking**: Assumes valid data

### Rendering Pipeline

1. **Load BSP**: Parse Quake 2 BSP format, extract geometry
2. **Texture Atlas**: Pack all textures into GL_TEXTURE_2D_ARRAY
3. **Lightmap Packing**: Atlas lightmaps into single 1024x1024 texture
4. **Vertex Format**: `[xyz, uv, lm_uv, tex_id, rgb]` (10 floats/vertex)
5. **Single Draw Call**: Render entire map with one `glDrawArrays`
6. **MD2 Animation**: CPU-side vertex interpolation between frames
7. **Post-Process**: Filmic tonemapping in fragment shader

### File Formats Supported

- **BSP (Binary Space Partition)**: Quake 2 maps with embedded geometry
- **MD2 (Quake 2 Model)**: Keyframe animation models
- **WAL (Wall Texture)**: Indexed color textures
- **PCX (PC Paintbrush)**: Palette and skin images
- **PAK (Pack File)**: Asset archive (extracted offline)

### Code Style

This is intentional code-golf style:
- Knuth-literate programming approach (dense but correct)
- Haskell-like functional composition where possible
- C99 features (compound literals, designated initializers)
- No external math libraries (manual matrix/vector ops)

## Demo Maps

The Quake 2 shareware includes:
- `demo1.bsp` - Industrial base (tested, works)
- `demo2.bsp` - Warehouse facility
- `demo3.bsp` - Communications center

## Performance

Tested configuration:
- **Map**: demo1.bsp
- **Geometry**: 22,448 triangles
- **Textures**: 581 unique textures
- **Lightmaps**: 1024x1024 atlas
- **Frame time**: ~16ms (60 FPS)

## Known Limitations

- No PVS (Potentially Visible Set) culling yet
- No skybox rendering (shows clear color)
- Limited entity support (player + one enemy + projectiles)
- No network multiplayer
- No save/load
- Auto-quits after 240 frames (testing mode)

## Screenshots

See `move_*.png` and `final.png` for gameplay screenshots.

## License

This is an educational/demonstration engine. Requires legitimate Quake 2 assets.
Original Quake 2 © id Software.

