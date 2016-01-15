//
// Created by matt on 1/13/16.
//

#ifndef CS5400_PUZZLE_H
#define CS5400_PUZZLE_H

#include <iostream>
#include <vector>
#include <queue>
#include <stack>

class Puzzle
{
private:
    //struct to define a flow
    struct Flow
    {
        int m_startx;
        int m_starty;
        int m_goalx;
        int m_goaly;
        int m_endx;
        int m_endy;
    };

    //struct to define a move
    struct Move
    {
        int m_depth;
        int m_flowID;
        int m_destx;
        int m_desty;
        int m_parent;
    };

    char** m_board;
    int m_height;
    int m_width;
    int m_numFlows;
    Move m_move;
    Flow* m_flows;
    void findEnd(int startX, int startY, int& x, int& y) const;
    void nextPiece(int& x, int& y) const;
    //int findID(int m_startx, int m_starty);
    //void nextPiece(int& x, int& y);

public:
    //Constructors/Destructors
    Puzzle();
    Puzzle(int h, int w, int flows);
    ~Puzzle();
    Puzzle(const Puzzle& rhs);

    //Overloaded operators
    Puzzle& operator=(const Puzzle& rhs);
    friend std::ostream& operator<<(std::ostream& os, const Puzzle& p);
    friend std::istream& operator>>(std::istream& is, Puzzle& p);

    bool isSolved() const;
    bool isValid(int x, int y);
    bool move(int dir, int x, int y, int id);

    //Solving methods
    Puzzle solveBFTS();
};

#endif //CS5400_PUZZLE_H
