#include "Node.h"
#include <iostream>

using namespace std;

Node::Node(int x,int y,int f,int g,int h) : x(x),y(y),f(f),g(g),h(h)
{
}

Node::Node(): x(0),y(0),f(0),g(0),h(0)
{

}

bool Node::operator<(const Node &other) const
{
    if(f<other.f)
        return true;
    else if(f==other.f)
        return g>other.g;

    return false;
}

bool Node::operator>(const Node &RHS) const
{
    return RHS<*this;//return true if this(the element on left) is greater than other (the one on RHS)
}

bool Node::operator==(const Node &other) const
{
    return (x==other.x && y==other.y);
}

bool Node::operator!=(const Node &other) const
{
    return !(*this==other);
}

void Node::printnode()
{
    cout<<"Node ("<<x<<","<<y<<") with f score : "<<f<<" and g score : "<<g<<endl;
}


Node::~Node()
{
}
