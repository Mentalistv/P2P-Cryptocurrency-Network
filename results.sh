#!/bin/bash

# Specify the directory containing CSV files
csv_directory="./outputs"

rm -f $csv_directory/*.pdf

# Check if the specified directory exists
if [ ! -d "$csv_directory" ]; then
    echo "Directory not found: $csv_directory"
    exit 1
fi

# Run the Python script for each CSV file in the directory
for csv_file in "$csv_directory"/*.csv; do
    if [ -f "$csv_file" ]; then
        echo "Processing file: $csv_file"
        python3 graph.py "$csv_file"
    else
        echo "No CSV files found in the directory: $csv_directory"
        exit 1
    fi
done
