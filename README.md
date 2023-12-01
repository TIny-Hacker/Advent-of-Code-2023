# Advent-of-Code-2023
My solutions for Advent of Code 2023, on the TI-84 Plus CE Graphing Calculator. I value my free time and don't have the brain power to figure out some of the later puzzles, so I probably won't do everything.

### grabInput.sh

This is a script I made to download a specified day's input and convert it to an 8xv file to send to the calculator. It also replaces all newlines (`0x0A`) with nulls (`0x00`) to make life easier. Syntax is as follows:

    ./grabInput.sh <year> <day>

It will create a directory called "Day\<day\>", with a converted `input.txt` and `Input.8xv`. It requires a file `cookies.txt` to be in the same directory as the script, which has the necessary info to allow you to properly download the input. You can generate a `cookies.txt` file with the cookies.txt extension for [Chrome](https://chrome.google.com/webstore/detail/get-cookiestxt/bgaddhkoddajcdgocldbbfleckgcbcid) or [Firefox](https://addons.mozilla.org/en-US/firefox/addon/cookies-txt/). The script uses `wget`, `tr`, and `convbin`.


### setupDirectories.sh

This script sets up my template directories for the specified year/day, along with creating a makefile. Syntax is as follows:

    ./setupDirectories.sh <year> <day>

It will generate a filestructure like this:

    ├── Day<day>
    │   ├── Part1
    │   │   ├── src
    │   │   ├── makefile
    │   ├── Part2
    │   │   ├── src
    │   │   ├── makefile

It will generate a template makefile something like this (note that the part will change based on the part directory it is in):

    NAME = AOC<day>
    DESCRIPTION = "Advent of Code <year>, Day <day>, Part 1"
    COMPRESSED = YES
    ARCHIVED = YES

    CFLAGS = -Wall -Wextra -Oz
    CXXFLAGS = -Wall -Wextra -Oz

    include $(shell cedev-config --makefile)
