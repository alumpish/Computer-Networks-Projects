#!/bin/bash

folder_path="plots"

# Find all text files in the plots folder
text_files=$(find "$folder_path" -type f -name "*.txt")

# Loop through each text file
for file in $text_files; do
    # Get the base name of the text file without the extension
    base_name=$(basename "$file" .txt)
    
    # Set the output file name based on the input file
    output_file="$folder_path/${base_name}.png"
    
    # Run gnuplot with the provided script and output file
    gnuplot -e "set terminal png; set output '$output_file'; set datafile separator ' '; \
                set xlabel 'Time'; set ylabel 'Value'; \
                plot '$file' using 1:2 with lines title 'cwnd', \
                '$file' using 1:3 with lines title 'sstresh'"
done
