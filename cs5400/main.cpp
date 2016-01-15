#include "Puzzle.h"

#include <iostream>
#include <fstream>

int main(int argc, char** argv)
{
    int width, height, flows;

    // Open puzzle file
    std::ifstream puzzle_file;
    if(argc == 1)
        puzzle_file.open("/home/matt/Documents/school/cs5400/puzzles/test.dat");
    else
        puzzle_file.open(argv[1]);

    //Create puzzle
    puzzle_file >> width;
    puzzle_file >> height;
    puzzle_file >> flows;
    Puzzle p(height, width, flows);
    puzzle_file >> p;

    p = p.solveBFTS();

    std::cout << p << std::endl;

    //Clean up
    puzzle_file.close();
}