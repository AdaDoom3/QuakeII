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

### Phase 2: BSP Loader ⚙
- [x] Parse BSP header
- [x] Load vertices, faces, edges
- [x] Load texinfo structure
- [ ] Triangulate faces properly
- [ ] Screenshot: test.png (cube room)

### Phase 3: BSP Renderer ✗
- [ ] Upload geometry to GPU
- [ ] Basic texture rendering
- [ ] Lightmap application
- [ ] Screenshot: Textured level

### Phase 4: Textures ✗
- [ ] WAL texture loader
- [ ] PCX image loader
- [ ] Palette handling
- [ ] Screenshot: Full textures

### Phase 5: Models ✗
- [ ] MD2 loader
- [ ] Frame interpolation
- [ ] Model rendering
- [ ] Screenshot: Character model

### Phase 6: Gameplay ✗
- [ ] Camera/movement
- [ ] Collision detection
- [ ] Input handling
- [ ] Screenshot: First-person view

### Phase 7: Audio ✗
- [ ] OpenAL initialization
- [ ] WAV loading
- [ ] 3D positional audio
- [ ] Screenshot: N/A

## Running

```bash
# Assuming you have Quake II demo data in baseq2/
./q2 +map demo1
```

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
