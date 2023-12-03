#!/bin/bash

if [[ ! -f "input" ]] # Ensure the file was downloaded
then
    echo "No input file found."
    exit 1
fi

echo "Converting newlines to null..."

tr '\n' '\0' < input > input.txt # Zero terminate newlines

rm input

echo "Converting output to 8xv..."

convbin -j bin -k 8xv -i input.txt -o SampleInput.8xv -n Input # Convert to appvar

echo "Restoring input..."

tr '\0' '\n' < input.txt > input # Restore readable input
mv input input.txt

if [[ ! -f "SampleInput.8xv" ]] # Ensure the files were created
then
    echo "Something went wrong."
    exit 1
fi

echo "Success!"
exit 0
