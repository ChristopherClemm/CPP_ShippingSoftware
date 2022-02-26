#include <iostream>
#include <fstream>
#include <string>
#include <vector> 
#include <queue>
#include <cmath>
#include "node.h"
#include "container.h"


class myComparator
{
public:
    int operator() (const Node* n1, const Node* n2)
    {
        return n1->cost > n2->cost;
    }
};


class myComparatorPath
{
public:
    int operator() (const Node* n1, const Node* n2)
    {
        return n1->cost > n2->cost;
    }
};

std::vector<std::vector<Container>> readinFile();
void containersToBeLoaded();
void containersToBeUnloaded();
void extendBoard();
void printShip(std::vector<std::vector<Container>>);
void balance(std::vector<std::vector<Container>>);
void updateQueue(std::priority_queue<Node*, std::vector<Node*>, myComparator>&, Node *, int , int, std::vector<std::vector<Container>>&);
int findPath(int, int, int, int,  std::vector<std::vector<Container>> );
int mannhatanDistancePath(int, int, int, int);
void printPath(Node *);


int main()
{
    std::cout << "started\n";
    std::vector<std::vector<Container>>ship = readinFile();
    //containersToBeLoaded();
    //containersToBeUnloaded();
    //std::cout << "done\n";
    printShip(ship);
    std::cout << ship.size() << "\n";
    std::cout << ship[0].size() << "\n";
    std::cout << findPath( 1,  1,  1,  5, ship);
    //std::cout << findPath( 0,  1,  0,  6, ship);
    //Read in the ship contents
    // Get what needs to come off / on

    // queue of things that needs to get on


    //balance 
    //balance(ship);
    //std::cout << mannhatanDistancePath(1,1,4,5);
    //create global ship view
    return 0;


}



std::vector<std::vector<Container>> readinFile(){

    std::ifstream inFile;
    std::string filename = "ShipCase4.txt";
    inFile.open(filename);
    std::vector<std::vector<Container>>ship;
    std::string tempString = "";
    int x = 0;
    int y = 0;
    //std::cout << "here\n";
    for(int i = 0; i < 8; i++)
    {
        std::vector<Container> tempStore;
        while(x < 12)
        {
            std::getline(inFile,tempString);
            Container tempContainer;
            tempContainer.x = i+1;
            tempContainer.y = x+1;
            tempContainer.contents = tempString.substr(18);
            if (tempContainer.contents == "NAN")
            {
                tempContainer.accessable = false;
                tempContainer.isContanier = false;
                tempContainer.isEmpty = false;
            }
            else
            {
                tempContainer.accessable = true;
                if (tempContainer.contents == "UNUSED")
                {
                    tempContainer.isContanier = false;
                    tempContainer.isEmpty = true;
                }
                else
                {
                    tempContainer.isContanier = true;
                    tempContainer.isEmpty = false;

                }
            }
            //std::cout <<  tempString.substr(10,5) << "\n";
            tempContainer.stringWeight = tempString.substr(10,5);
            tempContainer.weight = std::stoi(tempString.substr(10,5));
            //std::cout <<  tempContainer.weight << "\n";
            
            tempStore.push_back(tempContainer);
            
            x++;
        }
        ship.push_back(tempStore);
        x = 0;

    }
    //std::cout <<"here2";
    //std::cout << ship.size() << std::endl;
    //std::cout << ship[0].size() << std::endl;
    //STILL WANT TO ADD OTHER ROWS TO THE SHIP


    //Create pink Area
    for( int i = 1; i < 3; i++)
    {
        std::vector<Container> tempStore;
        for( int j = 0; j < 12; j++)
        {
            Container tempContainer;
            tempContainer.x = i+8;
            tempContainer.y = j+1;
            tempContainer.contents = "UNUSED";
            tempContainer.accessable = true;
            tempContainer.isContanier = false;
            tempContainer.isEmpty = true;
            tempContainer.stringWeight = "00000";
            tempContainer.weight = 0;
            tempStore.push_back(tempContainer);
        }
        ship.push_back(tempStore);
    }
    return ship;
    

}

void containersToBeLoaded()
{
    std::ifstream inFile;
    std::string filename = "onloadTest.txt";
    inFile.open(filename);
    std::vector<Container>Onload;
    std::string tempString = "";
    while(std::getline(inFile,tempString))
    {
        Container tempContainer;
        tempContainer.x = 0;
        tempContainer.y = 0;
        //still need to set weights
        tempContainer.contents = tempString.substr(9);
        //std::cout <<  tempContainer.contents << "\n";
        Onload.push_back(tempContainer);
    }

}
void containersToBeUnloaded()
{
    std::ifstream inFile;
    std::string filename = "unloadTest.txt";
    inFile.open(filename);
    std::vector<Container>Unload;
    std::string tempString = "";
    while(std::getline(inFile,tempString))
    {
        Container tempContainer;
        tempContainer.x = std::stoi(tempString.substr(1,2));
        tempContainer.y = std::stoi(tempString.substr(4,2));
        tempContainer.contents = tempString.substr(18);
        std::cout <<  tempContainer.x << "\n";
        std::cout <<  tempContainer.y << "\n";
        std::cout <<  tempContainer.contents << "\n";
        Unload.push_back(tempContainer);
    }

}

void extendBoard()
{
    //This function allows us to utilize the pink area above the ship and the buffer area on the shipyard


}

void printShip(std::vector<std::vector<Container>>ship)
{
    //This allows us to visualize the current state of the ship

    for( int i = 9; i >= 0; i--)
    {
        for (int j = 0; j < 12; j++)
        {
            //std::cout << i << " " << j << "\n";
            if(!ship[i][j].accessable )
            {
                std::cout << "NANNN" << " ";
            }
            else if (!ship[i][j].isContanier)
            {
                std::cout << "     " << " ";
            }
            else
            {
                std::cout << ship[i][j].stringWeight << " ";
            }
            
        }
        std::cout << "\n";
    }

}



void balance(std::vector<std::vector<Container>>ship)
{

  std::vector<Container>containerList;
  int sum = 0;
  for( int i = 7; i >= 0; i--)
    {
        for (int j = 0; j < 12; j++)
        {
            //std::cout << i << " " << j << "\n";
            if(ship[i][j].isContanier)
            {
                std::cout << ship[i][j].contents << "\n";
                containerList.push_back(ship[i][j]);
                sum += ship[i][j].weight;
            }
            
        }
        
    }
    std::cout << containerList.size() << "\n";
    std::cout << sum << "\n";
    //devolp the particion for the answers


    //misplaced tiles
    
    Node* currNode = new Node();
    currNode->currShip = ship;
    currNode->cost = 10;


    //Node* prev = *t;
    printShip(currNode->currShip);
    //  
    std::priority_queue<Node*, std::vector<Node*>, myComparator>pq;
    pq.push(currNode);
    Node* currNode1 = new Node();
    currNode1->currShip = ship;
    currNode1->cost = 100;
    pq.push(currNode1);
    Node* p = pq.top();
    std::cout << p->cost << "\n";

    int i = 0;
    long long nodesExpanded = 0;
    while (i < 100)
    {
        //if it is the goal state return true;

        //updateQueue(pq,currNode);

        nodesExpanded++;
        i++;
    }
   

}
//12*12 possible moves it could make 
int mannhatanDistancePath(int x1, int y1, int x2, int y2)
{
    return std::abs(double(x1)-x2) + std::abs(double(y1)-y2);
    

}



int findPath(int x1, int y1, int x2, int y2,  std::vector<std::vector<Container>> ship)
{
    
    int cost = 0;
    std::priority_queue<Node*, std::vector<Node*>, myComparator>pqPath;
    Node* currNode = new Node();
    //currNode->currShip = ship;
    currNode->cost = 0;
    int nodesExpanded = 0;
    currNode->currX = x1;
    currNode->currY = y1;
    currNode->depth = 0;
    pqPath.push(currNode);
    int currDepth = 0;
    while(pqPath.size() != 0)
    {
        currNode = pqPath.top();
        pqPath.pop();
        nodesExpanded++;
        if(currNode->currX == x2 && currNode->currY == y2)
        {
            std::cout << "Solution Found\n";
            break;
        }

        //need to create seen hash for less repeated states
        updateQueue(pqPath, currNode, x2, y2, ship);
        //currDepth++;
        //std::cout << nodesExpanded << "\n";



    }
    std::cout << nodesExpanded << "\n";
    printPath(currNode);
    return currNode->cost;

    


     
}

void updateQueue(std::priority_queue<Node*, std::vector<Node*>, myComparator>& pq, Node * currNode, int x2, int y2, std::vector<std::vector<Container>> &ship )
{
    //time to explore the different nodes
    
    //std::vector<std::vector<Container>> ship = currNode->currShip;
    int x = currNode->currX;
    int y = currNode->currY;
    //x, y to move down
    //oops x and y axis flipped
    if(x != 0)
    {
        if(ship[(x - 1)][y].isEmpty)
        {
            Node* leftNode = new Node();
            leftNode->prev = currNode;
            leftNode->currX = x-1;
            leftNode->currY = y;  
            leftNode->depth = currNode->depth+1;
            leftNode->cost = currNode->depth + 1 + mannhatanDistancePath((x - 1), y, x2, y2);
            pq.push(leftNode);
        }
    }
    //x, y to move up
    if(x != 9)
    {
        if(ship[(x + 1)][y].isEmpty)
        {
            Node* rightNode = new Node();
            rightNode->prev = currNode;
            rightNode->currX = x+1; 
            rightNode->currY = y;
            rightNode->depth = currNode->depth+1;
            rightNode->cost = currNode->depth + 1 + mannhatanDistancePath((x + 1), y, x2, y2);
            pq.push(rightNode);
        }
    }
    //x, y to move left
    if(y != 0)
    {
        if(ship[x][y-1].isEmpty)
        {
            Node* downNode = new Node();
            downNode->prev = currNode;
            downNode->currX = x;
            downNode->currY = y-1; 
            downNode->depth = currNode->depth+1;
            downNode->cost = currNode->depth + 1 + mannhatanDistancePath(x, (y-1), x2, y2);
            pq.push(downNode);
        }
    }
    //x, y to move right
    if(y != 7)
    {
        if(ship[x][y+1].isEmpty)
        {
            Node* upNode = new Node();
            upNode->prev = currNode;
            upNode->currX = x;
            upNode->currY = y+1; 
            upNode->depth = currNode->depth+1;
            upNode->cost = currNode->depth + 1 + mannhatanDistancePath(x, (y+1), x2, y2);
            pq.push(upNode);
        }
    }


}

void printPath(Node * currNode)
{
    while(currNode != NULL)
    {
        std::cout << currNode->currX << ", " << currNode->currY << "\n";
        currNode = currNode->prev;
    }
}