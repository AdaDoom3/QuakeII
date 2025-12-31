#!/usr/bin/env python3
import struct
import sys

def check_bsp(bsp_file):
    with open(bsp_file, 'rb') as f:
        sig = f.read(4)
        ver = struct.unpack('<I', f.read(4))[0]
        print(f"BSP signature: {sig}, version: {ver}")

        if sig != b'IBSP' or ver != 38:
            print("Not a valid Quake II BSP v38")
            return

        # Read lump directory (19 lumps)
        lumps = []
        for i in range(19):
            offset, length = struct.unpack('<II', f.read(8))
            lumps.append((offset, length))

        print(f"\nLump sizes:")
        lump_names = ['entities', 'planes', 'vertices', 'visibility', 'nodes',
                     'texinfo', 'faces', 'lightmaps', 'leaves', 'leaffaces',
                     'leafbrushes', 'edges', 'surfedges', 'models', 'brushes',
                     'brushsides', 'pop', 'areas', 'areaportals']

        for i, (name, (offset, length)) in enumerate(zip(lump_names, lumps)):
            print(f"  {i:2d}. {name:15s}: offset={offset:7d}, length={length:7d}")

        # Texinfo lump (5)
        texinfo_offset, texinfo_length = lumps[5]
        texinfo_size = 76  # sizeof(TI) from q2.c
        num_texinfo = texinfo_length // texinfo_size
        print(f"\nTexinfo entries: {num_texinfo}")

        f.seek(texinfo_offset)
        print(f"\nFirst 20 texture names:")
        for i in range(min(20, num_texinfo)):
            f.seek(texinfo_offset + i * texinfo_size)
            # Skip the first 32 bytes (v[2][4])
            f.read(32)
            flags, val = struct.unpack('<II', f.read(8))
            name = f.read(32).split(b'\x00')[0].decode('ascii', errors='ignore')
            print(f"  {i:3d}: {name}")

if __name__ == '__main__':
    check_bsp('baseq2/maps/demo1.bsp')
