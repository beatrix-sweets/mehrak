#!/usr/bin/env python3

import struct

# Set image dimensions manually (must match the raw file's size)
IMAGE_WIDTH = 64
IMAGE_HEIGHT = 64

# Read raw RGB data
with open("output.raw", "rb") as f:
    raw_data = f.read()

# Store non-black pixels
non_black_pixels = []

for y in range(IMAGE_HEIGHT):
    for x in range(IMAGE_WIDTH):
        index = (y * IMAGE_WIDTH + x) * 3
        r, g, b = struct.unpack("BBB", raw_data[index:index+3])

        # Only store non-black pixels
        if (r, g, b) != (0, 0, 0):
            non_black_pixels.append(f"{{{x}, {y}, 0x{r:02X}, 0x{g:02X}, 0x{b:02X}}}")

# Create the C header file
with open("image.h", "w") as f:
    f.write("#include <stdint.h>\n\n")
    f.write("typedef struct {\n    uint16_t x, y;\n    uint8_t r, g, b;\n} Pixel;\n\n")
    f.write(f"const Pixel image_data[{len(non_black_pixels)}] = {{\n    ")

    # Write pixel data with line breaks for readability
    for i, pixel in enumerate(non_black_pixels):
        if i > 0 and i % 5 == 0:  # Wrap lines every 5 values
            f.write("\n    ")
        f.write(pixel + ", ")

    f.write("\n};\n")

print(f"Header file 'image.h' generated with {len(non_black_pixels)} non-black pixels!")

