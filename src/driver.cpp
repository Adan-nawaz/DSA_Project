#include "driver.h"
#include "maze_gen.h"
#include <iostream>

using namespace std;

void driver::clearScreen()
{
    #ifdef _WIN32
    system("cls");

    #else
    system("clear");

    #endif
}

void driver::displayMenu()
{
    cout<<"Maze Solver"<<endl;
    cout<<"    1.Set New Maze"<<endl;
    cout<<"    2.Generate New Maze"<<endl;
    cout<<"    3.Display Maze"<<endl;
    cout<<"    4.Exit"<<endl;
    cout<<"Choose an option"<<endl;
}

void driver::run()
{
    clearScreen();
    //Here we are making maze generator object and as long as game runs this run function will run it wont return
    //This maze generator has a struct node and it will remain on stack till end of program no deletion once you have set the dimensions
    MazeGenerator generator(25,15);
    generator.Generate();
}




