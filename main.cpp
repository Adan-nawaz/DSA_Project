#include <iostream>
#include "driver.h"
#include <vector>
#include <queue>
#include <Node.h>
#include <minHeap.h>
#include "maze_gen.h"
#include "raylib.h"
using namespace std;
template <typename T>
struct point
{
    T x,y;
    point()
    {

    }

    point(T x1,T y1) : x(x1),y(y1) {}
};



void printmatrix(const vector<vector<point<int>>> &grid)
{
    for(int i=0;i<grid.size();i++)
    {
        for(int j=0;j<grid[0].size();j++)
        {
            cout<<"("<<grid[i][j].x<<","<<grid[i][j].y<<")";
        }
        cout<<endl;
    }
}


//function to get a vector of valid neighbours for BFS
//Takes the grid,the point we are on,visited matrix,parent matrix
//as arguments

vector<point<int>> neighbourchecker(point<int> p,const vector<vector<char>> &grid,vector<vector<bool>> &visited,
                                    vector<vector<point<int>>> &parent)
{

    vector<point<int>> neighlist;
    point<int> prev(0,0); //initialize a point named prev
    //set its x and y to the current point
    prev.x=p.x;
    prev.y=p.y;
    //dx array for traversing up and then down (in the first two iterations)
    //x here indicates the rows number
    int dx[4]={-1,1,0,0};
    //dy array for traversing right and then left (in the 3rd and 4th iterations)
    //y here indicates the column number
    int dy[4]={0,0,1,-1};

    //since we can only move in 4 directions (there can only be 4 valid neighbours at most)
    //loop 4 times to check each neighbour
    for(int i=0;i<4;i++)
    {
        //initialize the newpoint(this will calculate the possible neighbours) with all 0's
        point<int> pnew(0,0);
        //calculate x by adding the values in currentnode(the node we are on)
        //and adding with the array dx(to move up and down)
        pnew.x=p.x+dx[i];
        //calculate x by adding the values in currentnode(the node we are on)
        //and adding with the array dy(to move right and left)
        pnew.y=p.y+dy[i];
        //checks for the newnode to be considered a valid/traversable neighbour
        //if the x and y do not go out of boundary, and the char at that index
        //is not a # and the nodes is not visited before
        if(((pnew.x<=(grid.size()-1)) && (pnew.x>=(0))) &&
           ((pnew.y<=(grid[0].size()-1)) && (pnew.y>=(0))) &&
           (grid[pnew.x][pnew.y]==' ')&& !(visited[pnew.x][pnew.y]))
        {
            //mark it visited now
            visited[pnew.x][pnew.y]=true;
            //keep record of where you came from (its parent)
            parent[pnew.x][pnew.y]=prev;
            //and push it into the list of valid neighbours
            neighlist.push_back(pnew);
        }
    }
    //return the list
    return neighlist;

}

//fucntion to reverse the vector of points
void reverse(vector<point<int>> &p1)
{
    int i=0;
    int j=p1.size()-1;
    //keep looping till i<j
    while(i<j)
    {
        //perform swapping
        point<int> temp = p1[i];
        p1[i]=p1[j];
        p1[j]=temp;
        //increment and decrement i and j respectively
        i++;
        j--;
    }
}

//BFS takes the grid,start and end points as arguments
vector<point<int>> BFS(const vector<vector<char>> &grid,const point<int> &start,const point<int> &exit)
{
    //intialize a queue
    queue<point<int>> q1;
    //parent matrix for backtracking
    vector<vector<point<int>>> parent(grid.size(),vector<point<int>>(grid[0].size(),{-1,-1}));
    //visited matrix so we can keep track of visited nodes
    vector<vector<bool>> visited(grid.size(),vector<bool>(grid[0].size(),false));

    //enqueue the starting node
    q1.push({start.x,start.y});
     //mark the start visited
    visited[start.x][start.y]=true;
    //make the currenpoint = start
    point<int> currentpoint=start;
    //counter to keep tracks of total no of nodes explored
    int totalnodes=0;
    //keep exploring points till the queue becomes empty
    while (!q1.empty())
    {
        //dequeue the node at start
        currentpoint = q1.front();
        q1.pop();
        //increment the count
        totalnodes++;
        //if we have reached the goal breakout of the loop
        if(currentpoint.x==exit.x && currentpoint.y==exit.y)
        {
            break;
        }
        //get a list of neighbours
        vector<point<int>> neighbours = neighbourchecker(currentpoint,grid,visited,parent);

        for(int i = 0;i<neighbours.size();i++)
        {
            //Enqueue all the valid neighbours
            q1.push(neighbours[i]);
        }
    }


    vector <point<int>> path; //make a vector of points for saving the path

    //after the while loop the currenpoint will ultimately point to ending point
    //keep backtracking till we reach the starting node
    while(currentpoint.x!=start.x || currentpoint.y!=start.y)
    {
        //append the point at the end at each iteration
        path.push_back(currentpoint);
        //now set the currentpoint to parentpoint (Back tracking)
        currentpoint=parent[currentpoint.x][currentpoint.y];
    }
    //push the starting point aswell to make a full path
    path.push_back(start);

    //reverse the path (it would origianly be in reverse order)
    reverse(path);

    /*for(int i=path.size()-1;i>=0;i--)
    {
        cout<<"("<<path[i].x<<","<<path[i].y<<")";
    }*/

    cout<<endl<<"TOTAL NODES EXPLORED : "<<totalnodes<<endl;

    //return that path
    return path;
}


//function to get a vector of valid neighbours for A*
//Takes the grid,the node we are on,matrix g_score,visited matrix,parent matrix and exit point
//as arguments
vector<Node> AstarNeighbours(const vector<vector<char>> &grid,const Node &n,
                            vector<vector<int>> &g_score,
                            vector<vector<bool>> &visited,
                            vector<vector<point<int>>> &parent,
                            const point<int> &exit)
{
    //dx array for traversing up and then down (in the first two iterations)
    //x here indicates the rows number
    int dx[] ={-1,1,0,0};
    //dx array for traversing right and then left (in the 3rd and 4th iterations)
    //y here indicates the column number
    int dy[] ={0,0,1,-1};
    //make a list to store the valid neighbours
    vector<Node> neighlist;
    //set the previous node to n(current node will be used for
    //backtracking and calculating gscore for the newnode)
    Node prev=n;

    //since we can only move in 4 directions (there can only be 4 valid neighbours at most)
    //loop 4 times to check each neighbour
    for(int i=0;i<4;i++)
    {
        //initialize the newnode(this will calculate the possible neighbours) with all 0's
        Node newnode(0,0,0,0,0);
        //calculate x by adding the values in currentnode(the node we are on)
        //and adding with the array dx(to move up and down)
        newnode.x=n.x+dx[i];
        //calculate x by adding the values in currentnode(the node we are on)
        //and adding with the array dy(to move right and left)
        newnode.y=n.y+dy[i];
        //checks for the newnode to be considered a valid/traversable neighbour
        //if the x and y do not go out of boundary, and the char at that index
        //is not a # and the nodes is not visited before
        if(((newnode.x<=(grid.size()-1)) && (newnode.x>=(0))) &&
           ((newnode.y<=(grid[0].size()-1)) && (newnode.y>=(0))) &&
           (grid[newnode.x][newnode.y]==' ')&& !(visited[newnode.x][newnode.y]))
        {
            //mark it visited now
            visited[newnode.x][newnode.y]=true;
            //keep record of where you came from (its parent)
            parent[newnode.x][newnode.y]={prev.x,prev.y};
            //store the old g_score
            int g_old = g_score[newnode.x][newnode.y];
            //calculate the new g_score for that node now
            g_score[newnode.x][newnode.y]=g_score[prev.x][prev.y]+1;
            //store that new score in its g attribute
            newnode.g = g_score[newnode.x][newnode.y];
            //if the newly calculate g score is better than previous gscore
            if(newnode.g<g_old)
            {
            //calculate its estimated manhattan distance
            newnode.h = abs(newnode.x-exit.x)+abs(newnode.y-exit.y);
            //calculte f score for that node
            newnode.f = newnode.g+newnode.h;
            //and push it into the list of valid neighbours
            neighlist.push_back(newnode);
            }
            else //if the gscore is not better than previous gscore do nothing
            {
                continue;
            }

        }

    }
    //return the list of neighbours
    return neighlist;
}

//Astar takes the grid,start and end points as arguments
vector<point<int>> Astar(const vector<vector<char>> &grid,const point<int> &start, const point<int> &exit)
{

    minHeap<Node> PQ; //intialize a priority queue
    //parent matrix for backtracking
    vector<vector<point<int>>> parent(grid.size(),vector<point<int>>(grid[0].size(),{-1,-1}));
    //visited matrix so we can keep track of visited nodes
    vector<vector<bool>> visisted(grid.size(),vector<bool>(grid[0].size(),false));
    //g_score matrix to keep track of gscore(the distance from starting node) for each node
    //initially its highest number that can be stored in 32bits
    vector<vector<int>> g_score(grid.size(),vector<int>(grid[0].size(),INT_MAX));
    //initial gscore is 0 (since we are at start)
    int g = 0;
     //estimated manhattan distance from the goal and the current node (does not account for obstacles)
    int h = abs(start.x-exit.x)+abs(start.y-exit.y);
    //f score is measured by adding both(primary guider)
    int f = g+h;

    //create a specific node for A*
    Node n1(start.x,start.y,f,g,h);
    //mark the start visited
    visisted[start.x][start.y]=true;
    //set the gscore = 0
    g_score[start.x][start.y]=0;
    //take the node as current
    Node curr = n1;
    //enqueue that node
    PQ.enqueue(n1);
    //counter to keep tracks of total no of nodes explored
    int totalnodes=0;
    //keep exploring nodes till we reach the goal or the queue becomes empty
    while((curr.x!=exit.x || curr.y!=exit.y))
    {
        //dequeue the node with minimum f score
        //if f score are equal prioritize g score
        curr = PQ.dequeue();
        //increment the nodes explored
        totalnodes++;
        //get a list of neighbours
        vector<Node> neighbourlist = AstarNeighbours(grid,curr,g_score,visisted,parent,exit);
        for(int i=0;i<neighbourlist.size();i++)
        {
            PQ.enqueue(neighbourlist[i]); //Enqueue all the valid neighbours
        }
    }


    vector <point<int>> path; //make a vector of points for saving the path

     //make an object of point struct and set its's x and y to the exits' x and y
    point<int> currentpoint(exit.x,exit.y);


    //keep backtracking till we reach the starting node
    while(currentpoint.x!=start.x || currentpoint.y!=start.y)
    {
        //append the point at the end at each iteration
        path.push_back(currentpoint);
        //now set the currentpoint to parentpoint (Back tracking)
        currentpoint=parent[currentpoint.x][currentpoint.y];
    }
    //push the starting point aswell to make a full path
    path.push_back(start);

    //reverse the path(since we were appending to the end)
    reverse(path);


    cout<<endl<<"TOTAL NODES EXPLORED : "<<totalnodes<<endl;

    //return the path
    return path;
}
//flag for destination checking
bool found=false;
int nodes=0;
point<int> dest;
void track(vector<vector<char>>&vec,vector<vector<bool>>&visited,vector<vector<point<int>>>&parent,point<int> src)
{
    //restricting to explore the remaining last row nodes
     if(found)
        return;
    //check if we reach the destination or not
     if(src.x == vec.size() - 1) {
        //assigning destination
        dest = src;
        found=true;
        return;
    }
    //pointer to where neighbor of node will be store
    point<int> p;
    //arrays to find the all four neighbor
    int x_array[4]={-1,1,0,0};
    int y_array[4]={0,0,1,-1};
    //loop to check the valid neighbor
    for(int i=0;i<4;i++)
    {
        //not traversing the remaining last row neighbors if we have reach one
        if(found)
            return;
        //finding the neighbor
        p.x=src.x+x_array[i];
        p.y=src.y+y_array[i];
        //checking that valid neighbors
        if(p.y>=0&&p.y<vec[0].size()&&p.x>=0&&p.x<vec.size()&&visited[p.x][p.y]==false&&vec[p.x][p.y] != '#')
        {
            nodes++;
            //mark valid neighbors as visited
            visited[p.x][p.y]=true;
            //assign parents point ot neighbor
            parent[p.x][p.y]=src;
            //recursively repeating the process for each neighbour
            track(vec, visited, parent, p);
        }
    }
}
//dfs function to implement dfs algo
vector<point<int>> dfs(vector<vector<char>>&vec)
{
    //p storing the starting point
    point<int> p;
    //temporary point for backtracking
    point<int> temp;
    //staring point
    p.x=0;
    p.y=1;
    //path which dfs traverse
    vector<point<int>>path;
    //visited matrix
    vector<vector<bool>>visited(vec.size(),vector<bool>(vec[0].size(),false));
    //parent matrix for backtracking
    vector<vector<point<int>>>parent(vec.size(),vector<point<int>>(vec[0].size(),{-1,-1}));
    //mark starting true
    visited[0][1]=true;
    //calling track function
    track(vec,visited,parent,p);
    //storing the destination in vector
    path.push_back(dest);
    //track all the path via parent matrix until we reach -1
    while(!(parent[dest.x][dest.y].x == -1 && parent[dest.x][dest.y].y == -1))
        {
            temp=parent[dest.x][dest.y];
            dest=temp;
            path.push_back(temp);
        }
//we get the reverse path this loop will correct it
    for(int i=0;i<(path.size())/2;i++)
    {
        point<int> temporary=path[i];
        path[i]=path[path.size()-1-i];
        path[path.size()-1-i]=temporary;
    }

    return path;

}

int main()
{
   vector<vector<char>> Matrix = {
    {'#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
    {'#', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#', '#', '#', '#', '#', '#'},
    {'#', ' ', ' ', '#', '#', '#', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', '#', '#', '#', ' ', ' ', '#'},
    {'#', '#', ' ', '#', '#', '#', ' ', '#', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', '#', '#', '#'},
    {'#', ' ', ' ', '#', '#', '#', ' ', ' ', ' ', '#', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#', '#', '#', '#', ' ', '#', '#', ' ', '#', '#', '#', '#', '#', '#', ' ', '#'},
    {'#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', '#', '#', '#', '#', '#', '#', '#', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#'}
};



    int height;
    int width;
    int start_x;
    int start_y;
    int end_x;
    int end_y;
    int choice=0;
    cout << "Enter 1 to generate random grid: "<<endl;
    // takes input from user
    cout << "Enter 2 to generate grid of comparison: ";
    cin >> choice;
    cout<< endl;

    vector<vector<char>> maze;

    if(choice==1)
    {
    cout<<"Enter the height of maze"<<endl;
    cin>>height;
    cout<<"Enter the width of maze"<<endl;
    cin>>width;
    MazeGenerator generator;
    generator.SetDimensions(width,height);
    generator.Generate();
    start_x= 0;
    start_y = generator.return_start_y();
    end_x = height - 1;
    end_y = generator.return_end_y();
    // '#' = wall, ' ' = walkable path
    maze = generator.m_CurrentMaze.m_Grid;
    }
    else if(choice==2)
    {
       maze=Matrix;
       start_x=0;
       start_y=1;
       end_x=14;
       end_y=18;
    }



    cout<<endl<<"***********************BFS**************************"<<endl;
    vector<point<int>> BFSpath = BFS(maze,{start_x,start_y},{end_x,end_y});
    for(int i=0;i<BFSpath.size();i++)
    {
        cout<<"("<<BFSpath[i].x<<","<<BFSpath[i].y<<")";
    }
    cout<<endl<<"***********************ASTAR**************************"<<endl;
    vector<point<int>> Astarpath = Astar(maze,{start_x,start_y},{end_x,end_y});
    for(int i=0;i<Astarpath.size();i++)
    {
        cout<<"("<<Astarpath[i].x<<","<<Astarpath[i].y<<")";
    }

    //vector<vector<char>> v = generator.m_CurrentMaze.m_Grid;
   vector<point<int>>dfspath=dfs(maze);
    cout<<endl<<"***********************DFS**************************"<<endl;
   cout<<"TOTAL NODES EXPLORE : "<<nodes<<endl;
   for(int i=0;i<dfspath.size();i++)
   {
       cout<<"("<<dfspath[i].x<<","<<dfspath[i].y<<")"<<' ';
   }

    // Visualization Constants and Setup
    int rows = maze.size();
    int cols = maze[0].size();
    int cellSize = 20; // Smaller cell size to fit 4 mazes
    int mazeWidth = cols * cellSize;
    int mazeHeight = rows * cellSize;
    int padding = 20;
    int screenWidth = (mazeWidth * 2) + (padding * 3);
    int screenHeight = (mazeHeight * 2) + (padding * 3);
    InitWindow(screenWidth, screenHeight, "Maze Visualization: Split Screen");
    SetTargetFPS(60); //  FPS  smooth animation
    int frameCount = 0;
    int animationSpeed = 5; // Frames per step
    while (!WindowShouldClose())
    {
        frameCount++;
        int step = frameCount / animationSpeed; //0.12 sec
        BeginDrawing();
        ClearBackground(RAYWHITE);
        //set x and y top left to  maze
        int ox = padding;
        int oy = padding + 20;
        DrawText("Empty Maze", ox, oy - 20, 20, DARKGRAY);
        // Draw Grid
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                //draw each block by just multiplying it with cell size
                int x = ox + c * cellSize;
                int y = oy + r * cellSize;
                if (maze[r][c] == '#') DrawRectangle(x, y, cellSize, cellSize, BLACK);
                else DrawRectangle(x, y, cellSize, cellSize, LIGHTGRAY);
                DrawRectangleLines(x, y, cellSize, cellSize, DARKGRAY);
            }
        }
       //set x and y top left to  maze
        ox = padding * 2 + mazeWidth;
        oy = padding + 20;
        DrawText("BFS Path (Blue)", ox, oy - 20, 20, DARKGRAY);
        // Draw Grid
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                //draw each block by just multiplying it with cell size
                int x = ox + c * cellSize;
                int y = oy + r * cellSize;
                if (maze[r][c] == '#') DrawRectangle(x, y, cellSize, cellSize, BLACK);
                else DrawRectangle(x, y, cellSize, cellSize, LIGHTGRAY);
                DrawRectangleLines(x, y, cellSize, cellSize, DARKGRAY);
            }
        }

        // Draw BFS Path
        int limitBFS;
        //this mean not draw rectangles more then number of cells
        if(BFSpath.size()> step)
        {
         limitBFS = step;
        }
        else
        {
           limitBFS = BFSpath.size();
        }
        //drawing the bfs path
        for (int i = 0; i < limitBFS; i++) {
             DrawRectangle(ox + BFSpath[i].y * cellSize, oy + BFSpath[i].x * cellSize, cellSize, cellSize, BLUE);
        }

        // 3. Bottom-Left: DFS (Red)
         //set x and y top left to  maze
        ox = padding;
        oy = padding * 2 + mazeHeight+ 20;
        DrawText("DFS Path (Red)", ox, oy - 20, 20, DARKGRAY);
        // Draw Grid
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                //draw each block by just multiplying it with cell size
                int x = ox + c * cellSize;
                int y = oy + r * cellSize;
                if (maze[r][c] == '#') DrawRectangle(x, y, cellSize, cellSize, BLACK);
                else DrawRectangle(x, y, cellSize, cellSize, LIGHTGRAY);
                DrawRectangleLines(x, y, cellSize, cellSize, DARKGRAY);
            }
        }
        //this mean not draw rectangles more then number of cells
        int limitDFS;
        if(dfspath.size()>step)
        {
            limitDFS=step;
        }
        else
        {
            limitDFS=dfspath.size();
        }
        //drawing dfs path
        for (int i = 0; i < limitDFS; i++) {
             DrawRectangle(ox + dfspath[i].y * cellSize, oy + dfspath[i].x * cellSize, cellSize, cellSize, RED);
        }
        // 4. Bottom-Right: A* (Green)
        //set x and y top left to  maze
        ox = padding * 2 + mazeWidth;
        oy = padding * 2 + mazeHeight + 20;
        DrawText("A* Path (Green)", ox, oy - 20, 20, DARKGRAY);
        // Draw Grid
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                //draw each block by just multiplying it with cell size
                int x = ox + c * cellSize;
                int y = oy + r * cellSize;
                if (maze[r][c] == '#') DrawRectangle(x, y, cellSize, cellSize, BLACK);
                else DrawRectangle(x, y, cellSize, cellSize, LIGHTGRAY);
                DrawRectangleLines(x, y, cellSize, cellSize, DARKGRAY);
            }
        }
        // Draw A* Path
        int limitAstar ;
        //this mean not draw rectangles more then number of cells
        if(Astarpath.size()>step)
        {
            limitAstar=step;
        }
        else
        {
            limitAstar=Astarpath.size();
        }
        //drawing ASTAR path
        for (int i = 0; i < limitAstar; i++) {
             DrawRectangle(ox + Astarpath[i].y * cellSize, oy + Astarpath[i].x * cellSize, cellSize, cellSize, GREEN);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
