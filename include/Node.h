#ifndef NODE_H
#define NODE_H
#include <iostream>

using namespace std;
class Node
{
    public:
        int x,y,f,g,h;
        Node();
        Node(int x,int y,int f,int g,int h);
        virtual ~Node();

        bool operator<(const Node &other) const;
        bool operator>(const Node &other) const;
        bool operator==(const Node &other) const;
        bool operator!=(const Node &other) const;
        void printnode();


    protected:

    private:
};

#endif // NODE_H
