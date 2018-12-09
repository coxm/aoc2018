# aoc2018
Solutions to [Advent of Code](adventofcode.com) for 2018.


## Building
As with most CMake projects, run something like:

    mkdir build
    cd build
    cmake ..
    make

You'll have to copy your own input into an `./inputs/` directory too, e.g.
`./inputs/01.txt`. Then you can execute each day's program, e.g.:

    cd build
    ./day-01

Some programs require input, e.g. using environment variables. For
example:

    cd build
    PLAYERS=9 MARBLES=25 ./day-09
