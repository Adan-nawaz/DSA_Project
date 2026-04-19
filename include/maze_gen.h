#ifndef MAZE_GEN_H
#define MAZE_GEN_H
#include"maze.h"
#include <iostream>
#include <random>

using namespace std;

class MazeGenerator
{
    private:
        int m_Width;
        int m_Height;
        mt19937 m_RNG;
        void CarveMaze(int x, int y);
        void addEnteranceAndExit();
    public:
        Maze m_CurrentMaze;
        int return_end_y();
        int return_start_y();
        MazeGenerator();
        MazeGenerator(int w, int h);
        bool isBounds(int ax, int bound);
        void SetDimensions(int& w, int& h);
        void Generate();
        void Display();
        ~MazeGenerator();

};

#endif // MAZE_GEN_H
