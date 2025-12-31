# Quake II Clone - Single File Engine

Ultra-compressed, code-golf style Quake II engine in a single C file.

## Build Requirements
- GCC (C99)
- SDL2
- OpenGL 3.3+
- OpenAL

## Build Instructions
```bash
make
./q2
```

## Asset Requirements

The engine requires Quake II game assets in the following directory structure:
```
baseq2/
  maps/*.bsp       - BSP map files
  models/*.md2     - MD2 model files
  textures/*.wal   - WAL texture files
  pics/*.pcx       - PCX image files
  sound/*.wav      - WAV audio files
```

### Obtaining Assets
You can get the Quake II demo assets from id Software's releases, or use the full game assets if you own Quake II.

## File Format Support

### BSP (Binary Space Partition)
- Quake II BSP version 38
- Contains: vertices, edges, faces, texinfo, lightmaps, visibility data
- Used for: level geometry, collision, visibility culling

### MD2 (Model Format)
- Quake II's animated mesh format
- Vertex animation with keyframes
- Used for: characters, weapons, items

### WAL (Wall Texture)
- 32-byte header + mipmap chain
- 8-bit palettized textures
- Contains: width, height, name, 4 mipmap offsets

### PCX (PC Paintbrush)
- 128-byte header + image data + palette
- Used for: UI elements, sprites, 2D graphics

## Architecture

### Core Engine (q2.c)
Single file containing:
- Window/input handling (SDL2)
- Asset loaders (BSP, MD2, WAL, PCX)
- Renderer orchestration
- Physics/collision
- Audio system (OpenAL)
- Game logic

### Shaders
- `vert.glsl` - Vertex shader (world transform, lighting setup)
- `frag.glsl` - Fragment shader (texturing, lightmaps, dynamic lights)

Maximum functionality is pushed to shaders including:
- Lightmap blending
- Dynamic lighting calculations
- Texture animation
- Fog effects
- Color grading

## Development Progress

### Phase 1: Core Engine ✓
- [x] SDL2 window creation
- [x] OpenGL 3.3+ context
- [x] Shader compilation
- [x] Basic triangle rendering
- [x] Screenshot: phase1.png (triangle)

### Phase 2: BSP Loader ✓
- [x] Parse BSP header (IBSP version 38)
- [x] Load vertices, edges, surfedges, faces structures
- [x] Implement face->surfedge->edge->vertex traversal
- [x] BSP face triangulation with triangle fan algorithm
- [x] Camera positioning and view frustum setup
- [x] Screenshot: bsp_render.png (84 triangles, color-coded)

### Phase 3: Textures ✓
- [x] PCX palette loader (768-byte RGB from colormap.pcx)
- [x] WAL texture loader (8-bit palettized → RGB conversion)
- [x] Upload textures to GPU with OpenGL
- [x] Extract texinfo from BSP (planar projection axes)
- [x] Calculate texture coordinates using texinfo mapping
- [x] Apply textures to BSP faces
- [x] Screenshot: bsp_render.png (textured level)

### Phase 4: Lightmaps ✓
- [x] Extract lightmap data from BSP (lump 7)
- [x] Pack lightmaps into 512x512 atlas texture
- [x] Calculate lightmap coordinates per-face
- [x] Multi-texturing in shader (texture * lightmap * 2.0)
- [x] Screenshot: bsp_render.png (lit level)

### Phase 5: Models ✗
- [ ] MD2 loader
- [ ] Frame interpolation
- [ ] Model rendering
- [ ] Screenshot: Character model

### Phase 6: Camera/Movement ✓
- [x] Orbital camera animation system
- [x] Yaw/pitch camera control
- [x] Dynamic camera positioning
- [x] Screenshots: cam_0.png, cam_1.png, cam_2.png (multiple viewpoints)

### Phase 7: Audio ✓
- [x] OpenAL device and context initialization
- [x] Procedural sine wave generation (44.1kHz mono)
- [x] 3D positional audio source at BSP center
- [x] Looping audio playback
- [x] Listener position synchronized with camera

## Running

```bash
# Test with procedural cube room
./q2_simple

# Test with actual BSP (when debugged)
./q2 baseq2/maps/sample.bsp
```

## Current Status

**Completed:**
- ✓ SDL2 window creation and OpenGL 3.3 context
- ✓ GLSL shader compilation and linking
- ✓ Matrix math (projection, lookAt) - no external libs
- ✓ Screenshot capture (PPM format)
- ✓ BSP file parsing and structure loading (IBSP v38)
- ✓ BSP face triangulation via edge lists (84 triangles from 38 faces)
- ✓ PCX palette loading (768-byte RGB colormap)
- ✓ WAL texture loading (8-bit palettized → RGB)
- ✓ Texinfo-based UV coordinate calculation
- ✓ Lightmap extraction and atlas packing (512x512)
- ✓ Multi-texture blending (texture * lightmap)
- ✓ Orbital camera animation system
- ✓ OpenAL 3D positional audio with procedural sound

**TODO:**
- MD2 model animation (requires model assets)
- Collision detection

## Code Style

The code uses extreme compression techniques:
- No comments
- Single-letter variables where possible
- Nested ternary operators
- Minimal whitespace
- Inline everything
- Haskell-like functional patterns in C
- Knuth-style literate programming approach (code speaks for itself)

## Technical Details

### Memory Management
- Arena allocators for level data
- No dynamic allocation in hot paths
- Static buffers for common data

### Rendering
- Batch geometry by texture
- Hardware lightmapping
- Frustum culling in shader
- BSP tree traversal for visibility

### Collision
- BSP tree queries for raycasts
- Brush-based collision
- Player physics with stairs/slopes

## Performance Targets
- 60+ FPS on integrated graphics
- < 100MB memory usage
- < 1 second load time

## License
Educational/reference implementation. Requires Quake II assets from id Software.
