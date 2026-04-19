#ifndef MAZE_H
#define MAZE_H
#include <vector>
#include <string>

using namespace std;

struct Maze
{
   int m_Width;
   int m_Height;
   string m_Name;
   vector<vector<char>> m_Grid;
   //Default Constructor
   Maze()
   {
       m_Width = 0;
       m_Height = 0;
       m_Name = "Unnamed";
   }
   //Parametrized Constructor
   Maze(int w, int h)
   {
       m_Width = w;
       m_Height = h;
       m_Name = "Unnamed";
   }
};
#endif // MAZE_H
