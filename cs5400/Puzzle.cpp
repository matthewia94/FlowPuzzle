//
// Created by matt on 1/13/16.
//

#include "Puzzle.h"

void Puzzle::findEnd(int startX, int startY, int& x, int& y, int i) const
{
    int tempX = startX, tempY = startY;
    int length = 0;
    //trace the pipe
    do
    {
        x = tempX;
        y = tempY;
        if(tempX == m_flows[i].m_goalx && tempY == m_flows[i].m_goaly)
        {
            break;
        }
            std::cout<<x<<","<<y<<std::endl;
        nextPiece(tempX, tempY);
    }while(m_board[tempY][tempX] == 'v' || m_board[tempY][tempX] == '<' || m_board[tempY][tempX] == '>' ||
            m_board[tempY][tempX] == '^');
}

void Puzzle::nextPiece(int& x, int& y) const
{
    switch (m_board[y][x])
    {
        case '^':
        case 'U':
            y -= 1;
            break;
        case 'v':
        case 'D':
            y += 1;
            break;
        case '<':
        case 'L':
            x -= 1;
            break;
        case '>':
        case 'R':
            x += 1;
            break;
    }
}

Puzzle::Puzzle()
{
    //Initialize board
    m_board = NULL;
    m_flows = NULL;
    m_height = 0;
    m_width = 0;
    m_numFlows = 0;

    //Initialize previous move
    m_move.m_parent = -1;
    m_move.m_flowID = 0;
    m_move.m_destx = 0;
    m_move.m_desty = 0;
    m_move.m_depth = 0;
}

Puzzle::Puzzle(int h, int w, int flows)
{
    //Initialize board to empty
    m_height = h;
    m_width = w;
    m_numFlows = flows;

    //Initialize previous board state
    m_move.m_parent = -1;
    m_move.m_flowID = 0;
    m_move.m_destx = 0;
    m_move.m_desty = 0;
    m_move.m_depth = 0;

    //Create board array
    m_flows = new Flow[flows];
    m_board = new char*[h];
    for(int i=0; i < h; i++)
        m_board[i] = new char[w];
}

Puzzle::~Puzzle()
{
    for(int i=0; i < m_height; i++)
        delete m_board[i];
    delete[] m_board;

    delete m_flows;
}

Puzzle::Puzzle(const Puzzle& rhs)
{
    m_height = rhs.m_height;
    m_width = rhs.m_width;
    m_numFlows = rhs.m_numFlows;

    m_move = rhs.m_move;

    m_flows = new Flow[m_numFlows];
    m_board = new char*[m_height];
    for(int i=0; i < m_height; i++)
        m_board[i] = new char[m_width];

    for(int i=0; i < m_numFlows; i++)
    {
        m_flows[i] = rhs.m_flows[i];
    }

    for(int i=0; i < m_height; i++)
    {
        for(int j=0; j < m_width; j++)
            m_board[i][j] = rhs.m_board[i][j];
    }
}

Puzzle& Puzzle::operator=(const Puzzle& rhs)
{
    for(int i=0; i < m_height; i++)
        delete m_board[i];
    delete[] m_board;
    delete m_flows;

    m_height = rhs.m_height;
    m_width = rhs.m_width;
    m_numFlows = rhs.m_numFlows;

    m_move = rhs.m_move;

    m_flows = new Flow[m_numFlows];
    m_board = new char*[m_height];
    for(int i=0; i < m_height; i++)
        m_board[i] = new char[m_width];

    for(int i=0; i < m_numFlows; i++)
    {
        m_flows[i] = rhs.m_flows[i];
    }

    for(int i=0; i < m_height; i++)
    {
        for(int j=0; j < m_width; j++)
            m_board[i][j] = rhs.m_board[i][j];
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Puzzle& p)
{
    for(int i=0; i < p.m_height; i++)
    {
        for(int j=0; j < p.m_width; j++)
            os << p.m_board[i][j] << " ";
        os << std::endl;
    }
    return os;
}

std::istream& operator>>(std::istream& is, Puzzle& p)
{
    //Create count array to keep track of start or end of flow
    int* count = new int[p.m_numFlows];
    for(int i=0; i < p.m_numFlows; i++)
        count[i] = 0;

    //Store board into member board array
    for(int i=0; i < p.m_height; i++)
    {
        for(int j=0; j < p.m_width; j++)
            is >> p.m_board[i][j];
    }

    //Parse extra information such as flow start and end
    for(int i=0; i < p.m_height; i++)
    {
        for(int j=0; j < p.m_width; j++)
        {
            if(p.m_board[i][j] != 'e')
            {
                if(count[p.m_board[i][j] - '0'] == 0)
                {
                    p.m_flows[p.m_board[i][j]-'0'].m_startx = j;
                    p.m_flows[p.m_board[i][j]-'0'].m_starty = i;
                    p.m_flows[p.m_board[i][j]-'0'].m_endx = j;
                    p.m_flows[p.m_board[i][j]-'0'].m_endy = i;
                    count[p.m_board[i][j] - '0'] = count[p.m_board[i][j] - '0'] + 1;
                }
                else
                {
                    p.m_flows[p.m_board[i][j]-'0'].m_goalx = j;
                    p.m_flows[p.m_board[i][j]-'0'].m_goaly = i;
                }
            }
        }
    }

    delete count;

    return is;
}

bool Puzzle::isSolved() const
{
    bool solved = true;
    char startChar;

    //Check to make sure flow is complete
    for(int i=0; i < m_numFlows; i++)
    {
        if(m_flows[i].m_endx != m_flows[i].m_goalx || m_flows[i].m_endy != m_flows[i].m_goaly)
            solved = false;
    }

    return solved;
}

bool Puzzle::isValid(int x, int y, int id)
{
    bool valid = false;
    if(y < m_height && x < m_width && x >= 0 && y >= 0)
    {
        if(m_board[y][x] == 'e')
            valid = true;
        else if(m_flows[id].m_goalx == x && m_flows[id].m_goaly == y)
            valid = true;
    }
    return valid;
}

bool Puzzle::move(int dir, int x, int y, int id)
{
    bool success = false;
    switch(dir)
    {
        case 0:
            //up
            if(isValid(x, y - 1, id))
            {
                m_board[y-1][x] = 'v';
                m_flows[id].m_endy = y-1;
                success = true;
            }
            break;
        case 1:
            //right
            if(isValid(x + 1, y, id))
            {
                m_board[y][x+1] = '<';
                m_flows[id].m_endx = x+1;
                success = true;
            }
            break;
        case 2:
            //down
            if(isValid(x, y + 1, id))
            {
                m_board[y+1][x] = '^';
                m_flows[id].m_endy = y+1;
                success = true;
            }
            break;
        case 3:
            //left
            if(isValid(x - 1, y, id))
            {
                m_board[y][x-1] = '>';
                m_flows[id].m_endx = x-1;
                success = true;
            }
            break;
    }
    return success;
}

Puzzle Puzzle::solveBFTS()
{
    std::vector<Puzzle> parents;
    std::queue<Puzzle> frontier;
    std::stack<Puzzle> path;
    Puzzle temp;
    frontier.push(*this);

    while(!frontier.front().isSolved())
    {
        parents.push_back(frontier.front());
        for (int i = 0; i < m_numFlows; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                temp = frontier.front();
                if (temp.move(j, temp.m_flows[i].m_endx, temp.m_flows[i].m_endy, i)) {
                    temp.m_move.m_flowID = i;
                    temp.m_move.m_destx = temp.m_flows[i].m_endx;
                    temp.m_move.m_desty = temp.m_flows[i].m_endy;
                    temp.m_move.m_parent = parents.size() - 1;
                    frontier.push(temp);
                }
            }

        }
        frontier.pop();
    }

    std::cout << "a" << std::endl;

    temp = frontier.front();
    path.push(temp);
    while(temp.m_move.m_parent != -1)
    {
        temp = parents.at(temp.m_move.m_parent);
        path.push(temp);
    }

    return frontier.front();
}

bool Puzzle::solveDLTS(int limit, std::vector<Move>& parents)
{
    std::stack<Puzzle> frontier;
    Puzzle tempEval, temp;
    frontier.push(*this);
    bool goal = false;
    Move m;

    while(!frontier.empty() && !frontier.top().isSolved())
    {
        parents.push_back(frontier.top().m_move);
        tempEval = frontier.top();
        frontier.pop();
        if(tempEval.m_move.m_depth < limit)
        {
            for(int i=0; i < m_numFlows; i++)
            {
                //test each direction
                for(int j=0; j < 4; j++) {
                    temp = tempEval;
                    if (temp.move(j, temp.m_flows[i].m_endx, temp.m_flows[i].m_endy, i))
                    {
                        temp.m_move.m_flowID = i;
                        temp.m_move.m_destx = temp.m_flows[i].m_endx;
                        temp.m_move.m_desty = temp.m_flows[i].m_endy;
                        temp.m_move.m_depth++;
                        temp.m_move.m_parent = parents.size() - 1;
                        frontier.push(temp);
                    }
                }
            }
        }
    }

    if(!frontier.empty() && frontier.top().isSolved())
    {
        goal = true;
        m.m_depth = frontier.top().m_move.m_depth;
        m.m_flowID = frontier.top().m_move.m_flowID;
        m.m_destx = frontier.top().m_move.m_destx;
        m.m_desty = frontier.top().m_move.m_desty;
        m.m_parent = frontier.top().m_move.m_parent;
        parents.push_back(m);
        *this = frontier.top();
    }

    return goal;
}

void Puzzle::solveIDDLTS()
{
    int depth = 0;
    std::vector<Move> parents;
    std::stack<Move> path;
    Move temp;
    int steps = 0;

    while(!solveDLTS(depth, parents))
    {
        depth++;
        std::cout << depth << std::endl;
        parents.clear();
    }
}

int Puzzle::getCost()
{
    int cost = 0;
    for(int k = 0; k < m_height; k++)
    {
        for(int j = 0; j < m_width; j++)
        {
            if(m_board[j][k] == 'v' || m_board[j][k] == '<' || m_board[j][k] == '>' || m_board[j][k] == '^')
            {
                cost++;
            }
        }
    }
    return cost;
}


Puzzle Puzzle::solveUCTS()
{
    std::vector<Puzzle> parents;
    std::queue<Puzzle> frontier;
    std::stack<Puzzle> path;
    Puzzle temp;
    frontier.push(*this);

    while(!frontier.front().isSolved())
    {
        parents.push_back(frontier.front());
        for (int i = 0; i < m_numFlows; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                temp = frontier.front();
                std::cout<<"-------------------------------------------"<<std::endl;
                std::cout<<temp<<std::endl;                
                if (temp.move(j, temp.m_flows[i].m_endx, temp.m_flows[i].m_endy, i)) {
                    temp.m_move.m_flowID = i;
                    temp.m_move.m_destx = temp.m_flows[i].m_endx;
                    temp.m_move.m_desty = temp.m_flows[i].m_endy;
                    temp.m_move.m_parent = parents.size() - 1;
                    temp.m_cost = temp.getCost();
                    frontier.push(temp);
                }
            }

        }
        frontier.pop();
    }

    std::cout << "a" << std::endl;

    temp = frontier.front();
    path.push(temp);
    while(temp.m_move.m_parent != -1)
    {
        temp = parents.at(temp.m_move.m_parent);
        path.push(temp);
    }

    return frontier.front();  
}