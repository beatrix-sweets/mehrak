#!/usr/bin/env python3

import struct
import sys
import os
import string

def sanitize_name(name):
    # Allowed characters for C identifiers: a-z, A-Z, 0-9, and underscore.
    valid_chars = string.ascii_letters + string.digits + '_'
    sanitized = ''.join(c if c in valid_chars else '_' for c in name)
    # Ensure the name doesn't start with a digit.
    if sanitized and sanitized[0].isdigit():
        sanitized = '_' + sanitized
    return sanitized

def process_image(filename):
    IMAGE_WIDTH = 64
    IMAGE_HEIGHT = 64
    
    with open(filename, "rb") as f:
        raw_data = f.read()

    non_black_pixels = []
    for y in range(IMAGE_HEIGHT):
        for x in range(IMAGE_WIDTH):
            index = (y * IMAGE_WIDTH + x) * 3
            r, g, b = struct.unpack("BBB", raw_data[index:index+3])
            if (r, g, b) != (0, 0, 0):
                non_black_pixels.append(f"{{{x}, {y}, 0x{r:02X}, 0x{g:02X}, 0x{b:02X}}}")

    base_name = os.path.splitext(os.path.basename(filename))[0]
    image_name = sanitize_name(base_name)
    return non_black_pixels, image_name

def main():
    if len(sys.argv) < 2:
        print("Usage: python script.py <file1.raw> <file2.raw> ...")
        sys.exit(1)
    
    image_data_list = []
    for filename in sys.argv[1:]:
        pixels, image_name = process_image(filename)
        image_data_list.append((image_name, pixels))
    
    with open("images.h", "w") as f:
        f.write("#include <stdint.h>\n")
        f.write("#include <stddef.h>\n\n")
        f.write("typedef struct {\n    uint16_t x, y;\n    uint8_t r, g, b;\n} Pixel;\n\n")
        
        # Write each image's pixel data array.
        for image_name, pixels in image_data_list:
            f.write(f"const Pixel {image_name}_data[{len(pixels)}] = {{\n    ")
            for i, pixel in enumerate(pixels):
                if i > 0 and i % 5 == 0:
                    f.write("\n    ")
                f.write(pixel + ", ")
            f.write("\n};\n\n")
        
        # Write metadata struct using underscores.
        f.write("typedef struct {\n    const char *name;\n    const Pixel *data;\n    unsigned int size;\n} Image_t;\n\n")
        
        # Write the array of image metadata.
        f.write("const Image_t images[] = {\n")
        for image_name, pixels in image_data_list:
            f.write(f"    {{\"{image_name}\", {image_name}_data, {len(pixels)}}},\n")
        f.write("};\n\n")
        
        f.write(f"const size_t num_images = {len(image_data_list)};\n")
    
    print(f"Header file 'images.h' generated with {len(image_data_list)} images!")

if __name__ == "__main__":
    main()

