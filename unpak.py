#!/usr/bin/env python3
import struct
import os
import sys

def extract_pak(pak_path, out_dir):
    with open(pak_path, 'rb') as f:
        magic = f.read(4)
        if magic != b'PACK':
            print(f"Not a PAK file: {pak_path}")
            return

        dir_offset, dir_length = struct.unpack('<II', f.read(8))
        num_files = dir_length // 64

        print(f"Extracting {num_files} files from {pak_path}...")

        f.seek(dir_offset)
        for i in range(num_files):
            entry = f.read(64)
            name = entry[:56].decode('ascii').rstrip('\x00')
            file_offset, file_size = struct.unpack('<II', entry[56:64])

            out_path = os.path.join(out_dir, name)
            os.makedirs(os.path.dirname(out_path), exist_ok=True)

            pos = f.tell()
            f.seek(file_offset)
            data = f.read(file_size)
            f.seek(pos)

            with open(out_path, 'wb') as out:
                out.write(data)

            if i % 100 == 0:
                print(f"  {i}/{num_files}: {name}")

        print(f"Extracted {num_files} files to {out_dir}")

if __name__ == '__main__':
    extract_pak('pak0.pak', 'baseq2')
