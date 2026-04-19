#include "maze_gen.h"
#include <vector>
#include <algorithm>
using namespace std;


MazeGenerator::MazeGenerator()
{
    m_Width = 0;
    m_Height = 0;
    random_device rd;
    m_RNG = mt19937(rd());
}

MazeGenerator::MazeGenerator(int w, int h)
{
    m_Width = w;
    m_Height = h;
    random_device rd;
    m_RNG = mt19937(rd());
}

void MazeGenerator::SetDimensions(int& w, int& h)
{
    //To check whether dimensions are odd or not if not odd we will increment even value by 1 to make it odd
    if(w % 2== 0)
    {
        m_Width = w + 1;
        w++;
    }
    else
    {
        m_Width = w;
    }
    if(h % 2== 0)
    {
        m_Height = h + 1;
        h++;
    }
    else
    {
        m_Height = h;
    }
}

void MazeGenerator::Generate()
{
    //If my user enters negative dimension then we should not continue and we return the process
    if(m_Width <= 4 || m_Height <= 4)
    {
        cout<<"Sorry negative dimensions are not acceptable"<<endl;
        return;
    }
    //So when ever I have to create new maze this parametrized constructor produces a temporary object and its contents are copied into m_CurrentMaze and after that it deletes and our original object gets new state with new attributes
    m_CurrentMaze = Maze(m_Width, m_Height);
    random_device rd;
    m_RNG.seed(rd());
    //Grid is a vector of vector of characters so first we need to input number of rows which will be given by height and second input is another vector which will take in number of columns which is given by width and character to fill up the grid
    m_CurrentMaze.m_Grid.assign(m_CurrentMaze.m_Height, vector<char>(m_CurrentMaze.m_Width,'#'));
    //Vector helps us in resizing grid through the height and width we are changing in object and grid is dependent on width and height.
    CarveMaze(1,1);
    //To add enterance and exit in maze, Enterence will be top first but exit will be random
    addEnteranceAndExit();
    //Display the maze
    Display();
}

//This function will help us check that our algorithm doesnt knock down the walls of grid
bool MazeGenerator::isBounds(int ax, int bound)
{
    //If it doesnt reaches first column and last column we return true else false
    return (ax > 0) && (ax < bound - 1);
}

//This function will do the carving of maze.
void MazeGenerator::CarveMaze(int row, int col)
{
    //Due to recursive calls these arrays will be created again and again on the stack so we make them static for once so that they get memory once and we dont fill stack by giving them memory again and again.
    static int dx[] = { 0,  0, -2,  2 };
    static int dy[] = { -2, 2,  0,  0 };

    //This vector contains directions 0 index for upward movement , 1 for moving 2 units down , 2 for moving 2 units left and 3 for moving 2 units right
    static vector<int> dirs = {0,1,2,3};
    //The shuffle algorithm in our STL library will shuffle the elements of array so we get new directions everytime.
    std::shuffle(dirs.begin(), dirs.end(), m_RNG);

    m_CurrentMaze.m_Grid[row][col] = ' ';

    for (int i = 0; i < 4; i++)
    {
        int dir = dirs[i];

        // Calculate the new cell coordinates by moving two steps in the chosen direction
        int newCol = col + dx[dir];
        int newRow = row + dy[dir];

        // Check bounds and whether the target cell is still a wall
        if ((isBounds(newRow, m_CurrentMaze.m_Height)) &&(isBounds(newCol, m_CurrentMaze.m_Width)) &&(m_CurrentMaze.m_Grid[newRow][newCol] == '#'))
        {
            // Knock down the wall and fill the space with character ' '
            m_CurrentMaze.m_Grid[row + dy[dir] / 2][col + dx[dir] / 2] = ' ';

            // Recurse into the next cell
            CarveMaze(newRow, newCol);
        }
    }
}

void MazeGenerator::Display()
{
    if(m_CurrentMaze.m_Grid.empty())
    {
        cout<<"The dimensions are not set."<<endl;
        return;
    }
    else
    {
        for (int i = 0; i < m_Height; i++)
        {
            for (int j = 0; j < m_Width; j++)
            {
                cout << m_CurrentMaze.m_Grid[i][j];
            }
        cout<<""<<endl;
        }
    }
}

int MazeGenerator::return_start_y()
{
    int i;
    for(i = 0 ; i< m_Width; i++)
    {
        if(m_CurrentMaze.m_Grid[0][i]==' ')
        {
            break;
        }
    }
    return i;
}

int MazeGenerator::return_end_y()
{
    int i;
    for(i = 0 ; i< m_Width; i++)
    {
        if(m_CurrentMaze.m_Grid[m_Height - 1][i]==' ')
        {
            break;
        }
    }
    return i;
}

void MazeGenerator::addEnteranceAndExit()
{
    //Our entry to maze is always fixed we dont change it
    m_CurrentMaze.m_Grid[0][1] = ' ';
    std::vector<int> validExits;
    //We push all the spaces in 2nd last row in the vector
    for (int x = 1; x < m_CurrentMaze.m_Width; x =x + 2)
    {
        if (m_CurrentMaze.m_Grid[m_CurrentMaze.m_Height - 2][x] == ' ')
        {
            validExits.push_back(x);
        }
    }

    //If there are spaces in last row we randomly choose one of exits for the path out
    if (!validExits.empty())
    {
        int exitIndex = validExits[m_RNG() % validExits.size()];
        m_CurrentMaze.m_Grid[m_CurrentMaze.m_Height - 1][exitIndex] = ' ';
    }
}


MazeGenerator::~MazeGenerator()
{

}
