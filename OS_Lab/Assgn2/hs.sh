#!/bin/bash

# Check if a file name is passed as an argument
if [ $# -eq 0 ]
then
    echo "Error: No file name provided."
    exit 1
fi

# Save the file name in a variable
file="$1"

# Check if the file exists
if [ ! -f "$file" ]
then
    echo "Error: File $file not found."
    exit 1
fi

# Remove the initial 5 characters from the file
sed 's/^.\{5\}//' "$file" > "$file".tmp

# Overwrite the original file with the modified contents
mv "$file".tmp "$file"
