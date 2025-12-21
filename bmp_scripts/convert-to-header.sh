#! /bin/bash
## Uses ImageMagick to convert from BMP to RAW format, then uses a Python script
## to create the header file and adds a comment with the original filename

# Check if at least one file was provided
if [ $# -eq 0 ]; then
    echo "Error: No input files provided"
    echo "Usage: $0 file1.bmp [file2.bmp ...]"
    echo "Usage: (from the root directory of the project): bmp_scripts/convert-to-header.sh bmp_scripts/images/bmp/*"
    exit 1
fi

# Initialize array for raw files
declare -a raws_array

# Process each file provided as an argument
for input_file in "$@"; do
    # Check if input file exists
    if [ ! -f "$input_file" ]; then
        echo "Error: File '$input_file' not found"
        echo "Usage: $0 file1.bmp [file2.bmp ...]"
        echo "Usage: (from the root directory of the project): bmp_scripts/convert-to-header.sh bmp_scripts/images/bmp/*"
        exit 1
    fi

    # Generate output filename based on input filename
    filename=$(basename "$input_file")
    filename_no_ext="${filename%.*}"
    raw_output="bmp_scripts/images/raw/${filename_no_ext}.raw"

    echo "Processing: $input_file"
    
    # Convert to RAW
    if ! convert "$input_file" -depth 8 "rgb:$raw_output"; then
        echo "Error: Failed to convert ${input_file}"
        exit 1
    else
        # Add to array properly
        echo "generated raw: ${raw_output}"
        raws_array+=("$raw_output")
    fi
done

# Create a properly escaped list of input files
files_list=$(printf '%s ' $(basename -a "$@"))

# Pass all raw files to the Python script
if python3 bmp_scripts/scripts/generate_header_array.py "${raws_array[@]}"; then
    # Add original filenames as comment
    { echo "//from files: ${files_list}"; cat "main/images.h"; } > "main/images.h.tmp"
    mv "main/images.h.tmp" "main/images.h"
    echo "Successfully converted to images.h"
else
    echo "Error executing Python script"
    exit 1
fi
