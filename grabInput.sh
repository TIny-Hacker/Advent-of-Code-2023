#!/bin/bash

YEAR=$1
DAY=$2

if [[ $# -ne 2 ]] # Ensure proper number of arguments
then
    echo "Incorrect number of arguments supplied. Proper syntax is ./grabInput.sh <year> <day>"
    exit 1
fi

if [[ ! -f "cookies.txt" ]] # Ensure the user has a cookies.txt file
then
    echo "No cookies.txt file. Please generate a cookies.txt file and put it in the same directory as this script."
    exit 1
fi

echo "Preparing directories..."

if [[ ! -d "Day$DAY" ]] # Check if the directory has already been created, if not, create it
then
    mkdir Day$DAY
fi

cd Day$DAY

echo "Downloading input..."

wget --load-cookies=../cookies.txt "https://adventofcode.com/$YEAR/day/$DAY/input" # Download latest input

if [[ ! -f "input" ]] # Ensure the file was downloaded
then
    echo "Download failed. Your cookies.txt may be out of date."
    exit 1
fi

echo "Converting newlines to null..."

tr '\n' '\0' < input > input.txt # Zero terminate newlines

rm input

echo "Converting output to 8xv..."

convbin -j bin -k 8xv -i input.txt -o Input.8xv -n Input # Convert to appvar

echo "Restoring input..."

tr '\0' '\n' < input.txt > input # Restore readable input
mv input input.txt

if [[ ! -f "Input.8xv" ]] # Ensure the files were created
then
    echo "Something went wrong."
    exit 1
fi

echo "Success!"
exit 0
