#ifndef CONATINER_H
#define CONATINER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector> 

class Container{
    public: 
        std::string contents;
        int x;
        int y;
        int weight;
        std::string stringWeight;
        bool accessable;
        bool isContanier;
        bool toBeRemoved; 
        bool isEmpty;

};

#endif