#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector> 

#include "container.h"
class Node{
    public:
        int cost;
        std::vector<std::vector<Container>> currShip;
        //Node * prev;
        int man;
        int currX;
        int currY;
        int depth;
        int craneX;
        int craneY;
        int balance = 100;
        bool emptyCrane;
        std::string instructions;
        std::string rawInsructions;
        int done; 

        //std::vector<int> vec;
        //Node(std::vector<int>& ship);
        //Node(std::vector<std::vector<Container>>);
};





#endif