DESCRIPTION:
A simple version of Conway's Game of Life written in C using the nCurses
library.
Press "p" to puase the program and "q" to quit.

DEPENDENCIES:
The only dependence is nCurses, on Debian-like system you can install it
using "apt-get install libncurses5-dev"

COMPILATION:
You can compile cLife using:
gcc -lncurses -o clife clife.c engine.h engine.c

LICENSE:
This repo is covered by the Stallman's beard
http://www.gnu.org/licenses/gpl-3.0.txt
