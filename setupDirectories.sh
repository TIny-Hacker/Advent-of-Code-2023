#!/bin/bash

YEAR=$1
DAY=$2

if [[ $# -ne 2 ]] # Ensure proper number of arguments
then
    echo "Incorrect number of arguments supplied. Proper syntax is ./setupDirectories.sh <year> <day>"
    exit 1
fi

echo "Creating directories..."

if [[ ! -d "Day$DAY" ]] # Check if the directory has already been created, if not, create it
then
    mkdir Day$DAY
fi

cd Day$DAY

if [[ ! -d "Part1" ]]
then
    mkdir Part1
fi

if [[ ! -d "Part2" ]]
then
    mkdir Part2
fi

cd Part1

echo "Creating makefiles..."

if [[ ! -d "src" ]]
then
    mkdir src
fi

if [[ ! -f "makefile" ]]
then
    echo -e "NAME = AOC$DAY" > makefile
    echo -e "DESCRIPTION = \"Advent of Code $YEAR, Day $DAY, Part 1\"" >> makefile
    echo -e "COMPRESSED = YES" >> makefile
    echo -e "ARCHIVED = YES\n" >> makefile
    echo -e "CFLAGS = -Wall -Wextra -Oz" >> makefile
    echo -e "CXXFLAGS = -Wall -Wextra -Oz\n" >> makefile
    echo -e "include \$(shell cedev-config --makefile)" >> makefile
fi

cd ../Part2

if [[ ! -d "src" ]]
then
    mkdir src
fi


if [[ ! -f "makefile" ]]
then
    echo -e "NAME = AOC$DAY" > makefile
    echo -e "DESCRIPTION = \"Advent of Code $YEAR, Day $DAY, Part 2\"" >> makefile
    echo -e "COMPRESSED = YES" >> makefile
    echo -e "ARCHIVED = YES\n" >> makefile
    echo -e "CFLAGS = -Wall -Wextra -Oz" >> makefile
    echo -e "CXXFLAGS = -Wall -Wextra -Oz\n" >> makefile
    echo -e "include \$(shell cedev-config --makefile)" >> makefile
fi