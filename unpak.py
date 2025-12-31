#!/usr/bin/env python3
import struct
import os
import sys

def extract_pak(pak_file, dest_dir):
    with open(pak_file, 'rb') as f:
        sig = f.read(4)
        if sig != b'PACK':
            print(f"Not a PAK file: {sig}")
            return

        dir_offset, dir_length = struct.unpack('<II', f.read(8))
        num_entries = dir_length // 64

        print(f"PAK file: {num_entries} entries")

        f.seek(dir_offset)
        for i in range(num_entries):
            entry = f.read(64)
            filename = entry[:56].split(b'\x00')[0].decode('ascii')
            file_pos, file_len = struct.unpack('<II', entry[56:64])

            full_path = os.path.join(dest_dir, filename)
            os.makedirs(os.path.dirname(full_path), exist_ok=True)

            pos = f.tell()
            f.seek(file_pos)
            data = f.read(file_len)
            f.seek(pos)

            with open(full_path, 'wb') as out:
                out.write(data)

            if i < 10 or i % 100 == 0:
                print(f"  [{i+1}/{num_entries}] {filename} ({file_len} bytes)")

if __name__ == '__main__':
    extract_pak('pak0.pak', 'baseq2')
