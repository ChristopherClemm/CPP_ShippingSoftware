#include <iostream>
#include <fstream>
#include <string>
#include <vector> 
#include <queue>
#include <algorithm>    // std::sort
#include <functional>
#include <cmath>
#include <set>
#include <utility>
#include <unordered_set>
#include "node.h"
#include "container.h"


class myComparator
{
public:
    int operator() (const Node n1, const Node n2)
    {
        return n1.cost > n2.cost;
    }
};


struct hashFunctionPair
{
  size_t operator()(const std::pair<int , 
                    int> &x) const
  {
    //std::cout<< (x.first ^ x.second) << "\n";
    
    return x.first ^ x.second;
  }
};

class NodeLoad{
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
        int done = 5; 
        std::vector<Container> currLoad;
        std::vector<Container> currUnload;
       

        //std::vector<int> vec;
        //Node(std::vector<int>& ship);
        //Node(std::vector<std::vector<Container>>);
};

class myComparatorLoad
{
public:
    int operator() (const NodeLoad n1, const NodeLoad n2)
    {
        return n1.cost > n2.cost;
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
std::vector<Container> containersToBeLoaded();
std::vector<Container> containersToBeUnloaded();
void extendBoard();
void printShip(std::vector<std::vector<Container>>);
void balance(std::vector<std::vector<Container>>);
void updateQueue(std::priority_queue<Node, std::vector<Node>, myComparator>&, Node , int , int, std::vector<std::vector<Container>>&, std::unordered_set<std::pair<int,int>,hashFunctionPair>&);
int findPath(int, int, int, int,  std::vector<std::vector<Container>> );
int mannhatanDistancePath(int, int, int, int);
void printPath(Node *);
std::vector<std::pair<int,int>> findContianersToMove(std::vector<std::vector<Container>>);
std::vector<std::pair<int,int>> findOpenPlaces(std::vector<std::vector<Container>>);
void swapContainers( std::vector<std::vector<Container>>&, int, int, int, int);
void updateQueueBalance(std::priority_queue<Node, std::vector<Node>, myComparator>& , Node  ,std::vector<int>&, std::vector<int>&,  std::unordered_set<std::string>&, std::string);
int isBalanced(std::vector<std::vector<Container>>& , std::vector<int>, std::vector<int>, std::string);
std::string stringify(std::vector<std::vector<Container>>&,int,int);
void printInstructions(Node * );
int readMode();
std::vector<std::vector<Container>> GenerateSIFT(std::vector<std::vector<Container>>);
std::vector<std::vector<Container>> emptyShip(std::vector<std::vector<Container>>&);
bool equalShips(std::vector<std::vector<Container>>, std::vector<std::vector<Container>>);
std::string stringifyWeight(std::vector<std::vector<Container>>&);
void devParts(std::vector<std::vector<Container>>&);
void addVector(const std::vector<int>& );
void calaculatePartitions(int , int ,  std::vector<int>  );
bool canBeBalanced(std::vector<std::vector<Container>>&);
int getDistanceFromSIFT(std::vector<std::vector<Container>>& );


void loadAndUnload(std::vector<std::vector<Container>>, std::vector<Container>, std::vector<Container>);
void updateQueueLoad(std::priority_queue<NodeLoad, std::vector<NodeLoad>, myComparatorLoad>& , NodeLoad ,  std::unordered_set<std::string>& );
void removeContainer( std::vector<std::vector<Container>>&, int ,int );
int loadAStar(NodeLoad);

void addContainer(std::vector<std::vector<Container>>&,int ,int,std::vector<Container>& );


int main()
{
    //std::cout << "started\n";
    int mode = readMode();
    
    std::vector<std::vector<Container>>ship = readinFile();

    std::vector<std::vector<Container>>shipSIFT = GenerateSIFT(ship);
    std::cout << "IN remove LOAD AND UNLOAD\n";
    /*
    NodeLoad temp;
    temp.currShip = ship;
    temp.currUnload = containersToBeUnloaded();
    std::cout << "A* to get there is " << loadAStar(temp) << "\n";
    //return 1;
    */
    
    if(mode == 1)
    {
        std::cout << "IN MODE BALANCE\n"; 
        balance(ship);
    }
    if(mode == 2)
    {
        std::cout << "IN MODE LOAD AND UNLOAD\n";
        std::vector<Container> contLoad = containersToBeLoaded();
        std::vector<Container> contUnload = containersToBeUnloaded();
        loadAndUnload(ship, contLoad, contUnload);
    }
    
    int i = 0;
    
    return 0;


}


int readMode()
{
    std::ifstream inFile;
    std::string filename = "mode.txt";
    inFile.open(filename);
    std::string tempString = "";
    std::getline(inFile,tempString);
    return std::stoi(tempString);
}

std::vector<std::vector<Container>> readinFile(){

    std::ifstream inFile;
    std::string filename = "ShipCase5.txt";
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

std::vector<Container> containersToBeLoaded()
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
        tempContainer.stringWeight = tempString.substr(1,5);
        tempContainer.weight = std::stoi(tempString.substr(1,5));
        //std::cout <<  tempContainer.contents << "\n";
        Onload.push_back(tempContainer);
    }
    return Onload;

}
std::vector<Container> containersToBeUnloaded()
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
    return Unload;
}




void loadAndUnload(std::vector<std::vector<Container>> ship, std::vector<Container> contLoad, std::vector<Container> contUnload)
{
    //Mark for removal CAN DO FASTER
    for(int k = 0; k < contUnload.size(); k++)
    {
        for(int i = 0; i < ship.size(); i++)
        {
            for(int j = 0; j < ship[0].size(); j++)
            {
                if(contUnload[k].x == ship[i][j].x && contUnload[k].y == ship[i][j].y)
                {
                    ship[i][j].toBeRemoved = true;
                    //std::cout << ship[i][j].contents << "\n";
                }
            }
        }
    }

    
    NodeLoad currNode;
    currNode.currShip = ship;
    currNode.cost = 0;
    currNode.craneX = 8;
    currNode.craneY = 0;
    currNode.depth = 0;
    currNode.emptyCrane = true;
    currNode.instructions = "";
    currNode.rawInsructions = "";
    //currNode.done = 0;
    currNode.currLoad = contLoad;
    currNode.currUnload = contUnload;
    std::unordered_set<std::string> loadSet;
    std::string myString = stringify(currNode.currShip, currNode.craneX,currNode.craneY);
    std::priority_queue<NodeLoad, std::vector<NodeLoad>, myComparatorLoad>pqLoad;
    pqLoad.push(currNode);
    loadSet.insert(myString);
    
    long long nodesExpanded = 0;



    while (pqLoad.size() != 0)
    {
        
        
        currNode = pqLoad.top();
        pqLoad.pop();
        if(currNode.done == 0)
        {
            std::cout << "SOLUTION FOUND!" << "\n";
            break;
        }
        updateQueueLoad(pqLoad, currNode, loadSet);
        nodesExpanded++;
        if(nodesExpanded % 5000 == 0)
        {
            std::cout << "NODES EXPANDED: " << nodesExpanded << "\n";
            std::cout << "pqBalance.size() = " << pqLoad.size() <<  "\n";
            std::cout << "depth = " <<currNode.depth << "\n";
            std::cout << "stringified weight = " << stringifyWeight( currNode.currShip) << "\n";
            printShip(currNode.currShip);
        }
    }
    printShip(currNode.currShip);

    std::cout << currNode.instructions << "\n";

    std::cout << "TOTOAL COST " <<currNode.depth << "\n";
    std::cout << "NODES EXPANDED: " << nodesExpanded << "\n";

    std::cout << currNode.rawInsructions << "\n";


}


void updateQueueLoad(std::priority_queue<NodeLoad, std::vector<NodeLoad>, myComparatorLoad>& pqLoad, NodeLoad currNode,  std::unordered_set<std::string>&loadSet)
{
    std::vector<std::vector<Container>> ship = currNode.currShip;
    
    
    if(currNode.done == 0)
    {
        std::cout << "DONE\n";
    }
    //removal spot is at 8,0 where we just add 15 to the cost 
    //pick up spot is at 8,0 where we just add 15 to the cost 

    int craneX = currNode.craneX;
    int craneY = currNode.craneY;
    int nodesAdded = 0;
    if(currNode.emptyCrane == true)
    {
        std::vector<std::pair<int,int>>containerToAccess =  findContianersToMove(ship);
          if(currNode.currLoad.size() > 0)
            {
                std::pair<int, int>myPair = std::make_pair(-2,-2);
                containerToAccess.push_back(myPair);

            }
        for( int i = 0; i < containerToAccess.size(); i++)
        {
            std::pair<int,int>currContianerPair = containerToAccess[i];

            if(currContianerPair.first == -2 && currContianerPair.second == -2)
            {

                int costToGetToContainer = 15 + findPath(craneX, craneY, 8, 0, ship);

                


                NodeLoad newNode;
                std::string craneXString = std::to_string(craneX+1);
                std::string craneYString = std::to_string(craneY+1);
                if(craneX+1 < 10)
                {
                    craneXString = "0"  + craneXString;
                } 
                if(craneY+1 < 10)
                {
                    craneYString = "0" + craneYString;
                } 

                std::string currXString = std::to_string(8+1);
                std::string currYString = std::to_string(0+1);
                if(currContianerPair.first+1 < 10)
                {
                    currXString = "0"  + currXString;
                } 
                if(currContianerPair.second+1 < 10)
                {
                    currYString = "0" + currYString;
                } 
                    
                
                newNode.currShip = ship;
                //add Container to ship
                newNode.currLoad = currNode.currLoad;
                
                addContainer(newNode.currShip,8,0, newNode.currLoad);
                //int balance = isBalanced(newNode.currShip,left,right, shipSIFT);
                newNode.emptyCrane = false;
                newNode.craneX = 8;
                newNode.craneY = 0;
                std::string tempString = stringify(newNode.currShip,newNode.craneX,newNode.craneY);
                if (loadSet.find(tempString)  == loadSet.end() )
                {
                    //std::cout << "check when crane = true" << "\n";
                    int aStarCost = loadAStar(newNode);
                    //newNode.done = aStarCost;
                    newNode.depth = currNode.depth + costToGetToContainer;
                    newNode.cost = currNode.depth + costToGetToContainer + aStarCost;
                    newNode.instructions = currNode.instructions + "PICK UP Move Crane from (" + std::to_string(craneX) + ", " +  std::to_string(craneY)  + ") to (" +  std::to_string(currContianerPair.first) + " , " 
                        +  std::to_string(currContianerPair.second)+ ")\n";
                    newNode.rawInsructions = currNode.rawInsructions + "[" + craneXString + "," + craneYString  + "] [00,00] \n";
                    newNode.currUnload = currNode.currUnload;
                    

                    pqLoad.push(newNode);
                    loadSet.insert(tempString);
                    nodesAdded++;
                }


            }
            else{

                //COST TO GET TO THE CONTAINER
                int costToGetToContainer = findPath(craneX, craneY, currContianerPair.first, currContianerPair.second, ship);
                NodeLoad newNode;
                std::string craneXString = std::to_string(craneX+1);
                std::string craneYString = std::to_string(craneY+1);
                if(craneX+1 < 10)
                {
                    craneXString = "0"  + craneXString;
                } 
                if(craneY+1 < 10)
                {
                    craneYString = "0" + craneYString;
                } 

                std::string currXString = std::to_string(currContianerPair.first+1);
                std::string currYString = std::to_string(currContianerPair.second+1);
                if(currContianerPair.first+1 < 10)
                {
                    currXString = "0"  + currXString;
                } 
                if(currContianerPair.second+1 < 10)
                {
                    currYString = "0" + currYString;
                } 
                    
                
                newNode.currShip = ship;
                //int balance = isBalanced(newNode.currShip,left,right, shipSIFT);
                newNode.emptyCrane = false;
                newNode.craneX = currContianerPair.first;
                newNode.craneY = currContianerPair.second;
                std::string tempString = stringify(newNode.currShip,newNode.craneX,newNode.craneY);
                if (loadSet.find(tempString)  == loadSet.end() )
                {
                    //std::cout << "check when crane = true" << "\n";
                    newNode.depth = currNode.depth + costToGetToContainer;
                    newNode.cost =currNode.depth + 1 + costToGetToContainer;
                    newNode.instructions = currNode.instructions + "Move Crane from (" + std::to_string(craneX) + ", " +  std::to_string(craneY)  + ") to (" +  std::to_string(currContianerPair.first) + " , " 
                        +  std::to_string(currContianerPair.second)+ ")\n";
                    newNode.rawInsructions = currNode.rawInsructions + "[" + craneXString + "," + craneYString  + "] [" +  currXString + "," 
                        +  currYString+ "] \n";
                    newNode.currUnload = currNode.currUnload;
                    newNode.currLoad = currNode.currLoad;

                    pqLoad.push(newNode);
                    loadSet.insert(tempString);
                    nodesAdded++;
                }
            
            }
    
        }

    }
    else{
        std::vector<std::pair<int,int>>openSpacesToAccess =  findOpenPlaces(ship);
      
        if(ship[craneX][craneY].toBeRemoved)
        {
            //std::cout << "TO BE REMOVED " << "\n";

            std::pair<int, int>myPair = std::make_pair(-1,-1);
            openSpacesToAccess.push_back(myPair);
        }
        for(int j = 0; j < openSpacesToAccess.size(); j++)
        {
            
            std::pair<int,int>currEmptyPair = openSpacesToAccess[j];

            if(currEmptyPair.first == -2 && currEmptyPair.second == -2)
            {
                int cost = findPath(craneX, craneY, 8, 0, ship);
            }


            if(currEmptyPair.first == -1 && currEmptyPair.second == -1)
            {
                //std::cout << "STARTED TO BE REMOVED " << "\n";
                int cost = findPath(craneX, craneY, 8, 0, ship);
                NodeLoad newNode;
                //remove the contianer from the ship
                newNode.instructions = currNode.instructions + "Unload ("+  std::to_string(craneX) + " , "+  std::to_string(craneY)+ ")\n";
                newNode.currShip = ship;
                std::cout << "currUnload size  " << currNode.currUnload.size() <<"\n";
                for(int i = 0; i < currNode.currUnload.size(); i++)
                {
                    //std::cout << "STARTED TO BE REMOVED2 " << currNode.currUnload[i].x << "\n";
                   // std::cout << "STARTED TO BE REMOVED3\n";
                    if((currNode.currUnload[i].x != newNode.currShip[craneX][craneY].x) || (currNode.currUnload[i].y != newNode.currShip[craneX][craneY].y ))
                    {
                        //std::cout << "NOT EQUAL\n";
                        //std::cout << currNode.currUnload[i].contents << "\n";
                        newNode.currUnload.push_back(currNode.currUnload[i]);
                    }
                    //std::cout << "\n";
                    
                }
                std::cout << "STARTED TO BE REMOVED " << "\n";
                removeContainer(newNode.currShip, craneX,craneY);
                //std::cout << "STARTED TO BE REMOVED " << "\n";
                std::string craneXString = std::to_string(craneX+1);
                std::string craneYString = std::to_string(craneY+1);
                if(craneX+1 < 10)
                {
                    craneXString = "0"  + craneXString;
                } 
                if(craneY+1 < 10)
                {
                    craneYString = "0" + craneYString;
                } 

                std::string currXString = std::to_string(currEmptyPair.first+1);
                std::string currYString = std::to_string(currEmptyPair.second+1);
                if(currEmptyPair.first+1 < 10)
                {
                    currXString = "0"  + currXString;
                } 
                if(currEmptyPair.second+1 < 10)
                {
                    currYString = "0" + currYString;
                } 
                
                
                newNode.rawInsructions = currNode.rawInsructions + "[" + craneXString + "," + craneYString +"] ["  +  currXString + "," + currYString + "]\n";
                std::string tempString = stringify(newNode.currShip,newNode.craneX,newNode.craneY);
                if (loadSet.find(tempString)  == loadSet.end() )
                {
                    newNode.currLoad = currNode.currLoad;

                    int aStarCost = loadAStar(newNode);
                    newNode.done = aStarCost;
                    
                    newNode.craneX = 8;
                    newNode.craneY = 0;
                    newNode.emptyCrane = true;
                    //std::cout << "NEW NODE balance  : " << newNode.balance << "\n";
                    newNode.depth = currNode.depth + cost;
                    //std::cout << "NEW NODE depth  : " << newNode.depth << "\n";
                    newNode.cost = currNode.depth + 1 + cost + aStarCost;
                    if(aStarCost == 0)
                    {
                        newNode.cost = -10;
                    }
                    //std::cout<< myPair.first << ", " << myPair.second << "\n";            
                    loadSet.insert(tempString);
                    //std::cout << "NEW NODE COST  : " << newNode.cost << "\n";
                    pqLoad.push(newNode);
                    nodesAdded++;
                }


            }
            //Get cost of crane to this container 
            if (craneY != currEmptyPair.second)
            {
                
                //std::cout << findPath(currContianerPair.first, currContianerPair.second, currEmptyPair.first, currEmptyPair.second, ship) << "\n";
                int cost = findPath(craneX, craneY, currEmptyPair.first, currEmptyPair.second, ship);
                //std::cout << "FINDING WHERE TO PUT\n";
                NodeLoad newNode;
                //std::cout << "FINDING WHERE TO PUT\n";
                newNode.instructions = currNode.instructions + "Move ("+  std::to_string(craneX) + " , "+  std::to_string(craneY)+ ") to ("
                 +  std::to_string(currEmptyPair.first) + " , " +  std::to_string(currEmptyPair.second) + ")\n";

                std::string craneXString = std::to_string(craneX+1);
                std::string craneYString = std::to_string(craneY+1);
                if(craneX+1 < 10)
                {
                    craneXString = "0"  + craneXString;
                } 
                if(craneY+1 < 10)
                {
                    craneYString = "0" + craneYString;
                } 

                std::string currXString = std::to_string(currEmptyPair.first+1);
                std::string currYString = std::to_string(currEmptyPair.second+1);
                if(currEmptyPair.first+1 < 10)
                {
                    currXString = "0"  + currXString;
                } 
                if(currEmptyPair.second+1 < 10)
                {
                    currYString = "0" + currYString;
                } 
                
                
                newNode.rawInsructions = currNode.rawInsructions + "[" + craneXString + "," + craneYString +"] ["  +  currXString + "," + currYString + "]\n";
                //newNode->prev = currNode;
                //leftNode->currX = x;
                //leftNode->currY = y;  
                //std::cout << "COST  : " << cost << "\n";
                newNode.currShip = ship;
                swapContainers(newNode.currShip,craneX, craneY, currEmptyPair.first,currEmptyPair.second);
                std::string tempString = stringify(newNode.currShip,newNode.craneX,newNode.craneY);
                if (loadSet.find(tempString)  == loadSet.end() )
                {
                    newNode.currUnload = currNode.currUnload;
                    newNode.currLoad = currNode.currLoad;
                    int aStarCost = loadAStar(newNode);
                    newNode.done = aStarCost;
                    newNode.craneX = currEmptyPair.first;
                    newNode.craneY = currEmptyPair.second;
                    newNode.emptyCrane = true;
                    //std::cout << "NEW NODE balance  : " << newNode.balance << "\n";
                    newNode.depth = currNode.depth + cost;
                    //std::cout << "NEW NODE depth  : " << newNode.depth << "\n";
                    newNode.cost = currNode.depth + 1 + cost + aStarCost;
                    if(aStarCost == 0)
                    {
                        newNode.cost = -10;
                    }
                    //std::cout<< myPair.first << ", " << myPair.second << "\n";            
                    loadSet.insert(tempString);
                    //std::cout << "NEW NODE COST  : " << newNode.cost << "\n";
                    pqLoad.push(newNode);
                    nodesAdded++;
                }
                 
            }
        }


    }

}


int loadAStar(NodeLoad currNode)
{
    //Check how long it would take to load everything and unload everything by finding path + 15 to load and unload
    int sum = 0;
    for(int i = 0; i < currNode.currUnload.size(); i++)
    {
        //std::cout << currNode.currUnload[i].contents << "\n";
        for( int x = 0; x < currNode.currShip.size(); x++)
        {
            //std::cout << currNode.currUnload[i].contents << "1\n";
            for( int y = 0; y < currNode.currShip[0].size(); y++)
            {
                //std::cout << currNode.currUnload[i].contents << "2\n";
                if((currNode.currUnload[i].x == currNode.currShip[x][y].x) && ( currNode.currUnload[i].y== currNode.currShip[x][y].y))
                {
                    sum += findPath(x,y,8,0,currNode.currShip) + 15;
                }
            }
        }
    }
    for(int i = 0; i < currNode.currLoad.size(); i++)
    {
        sum += 15;
    }
    if(sum == 0)
    {
        //printShip(currNode.currShip);
    }
    //std::cout << " sum = " << sum << "\n";
    return sum;
}
void devParts(std::vector<std::vector<Container>>& ship)
{
    int leftSum = 0;
    int rightSum = 0;
    std::vector<Container>leftShip;
    std::vector<Container>rightShip;
    for( int i = 9; i >= 0; i--)
    {
        for (int j = 0; j < 12; j++)
        {
           if(ship[i][j].isEmpty || !ship[i][j].accessable)
           {
               continue;
           }
           if(j <= 5)
           {
               leftShip.push_back(ship[i][j]);
               leftSum += ship[i][j].weight;
               //std::cout << "left ship " << ship[i][j].weight << " "<< i << " , " <<  j << "\n";
           }
           else
           {
               rightShip.push_back(ship[i][j]);
               rightSum += ship[i][j].weight;
               //std::cout << "right ship " << ship[i][j].weight << " "<< i << " , " <<  j << "\n";
           }
            
        }
    }



    int totalSum = rightSum+leftSum;
    
    double diff = abs(double(rightSum) - leftSum);

    //std::cout << "diff = " << diff << "\n";
    double percent = 15;
    if(rightSum >= leftSum)
    {
         percent = ((diff/double(rightSum)) * 100);
    }
    else
    {
         percent = ((diff/double(leftSum)) * 100);
    }
    double allowance = totalSum*.1;
    int sum = totalSum;
    int size = (leftShip.size() + rightShip.size());
    


}
void extendBoard()
{
    //This function allows us to utilize the pink area above the ship and the buffer area on the shipyard


}

std::vector<std::vector<Container>> emptyShip(std::vector<std::vector<Container>>& ship)
{
    std::vector<std::vector<Container>> emptyShipNew;
    for(int i = 0; i < 10; i++)
    {
        std::vector<Container> tempStore;
        int x = 0;
        while(x < 12)
        {
            if(ship[i][x].accessable == false)
            {
                tempStore.push_back(ship[i][x]);
            }
            else
            {
            Container tempContainer;
            tempContainer.x = i+1;
            tempContainer.y = x+1;
            tempContainer.contents = "UNUSED";
            tempContainer.accessable = true;
            tempContainer.isContanier = false;
            tempContainer.isEmpty = true;
            tempContainer.stringWeight = "00000";
            tempContainer.weight = 0;
            tempStore.push_back(tempContainer);
                
            }
            //std::cout <<  tempString.substr(10,5) << "\n";
            //tempContainer.stringWeight = tempString.substr(10,5);
            //tempContainer.weight = std::stoi(tempString.substr(10,5));
            //std::cout <<  tempContainer.weight << "\n";
            
            
            
            
            x++;
        }
        x=0;
        emptyShipNew.push_back(tempStore);
    }
    //printShip(emptyShipNew);
    return emptyShipNew;
}


std::vector<std::vector<Container>> GenerateSIFT(std::vector<std::vector<Container>> ship)
{
    std::vector<Container> siftShip;
    std::vector<std::vector<Container>> finalSiftShip = emptyShip(ship);
    printShip(finalSiftShip);
    printShip(ship);


    for( int i = 9; i >= 0; i--)
    {
        for (int j = 0; j < 12; j++)
        {
           if(ship[i][j].isEmpty || !ship[i][j].accessable)
           {
               continue;
           }
           siftShip.push_back(ship[i][j]);
            
        }
    }
    sort( siftShip.begin( ), siftShip.end( ), [ ]( const Container lhs, const Container rhs )
    {
        return lhs.weight > rhs.weight;
    });
    int row = 0;
    int columnLeft = 5;
    int columnRight = 6;
    bool left = true;
    for(int i = 0; i < siftShip.size(); i++)
    {
        //std::cout << "TRYING TO PLACE " << siftShip[i].stringWeight << "\n";
        if(left)
        {
            if(!finalSiftShip[row][columnLeft].accessable)
            {
                //std::cout << "NAN in the way\n ";
                row++;
                columnLeft = 5;
                columnRight = 6;
                left = true;
                i--;

            }
            else{
                //std::cout << "row = "<< row << " , left col = " << columnLeft << "\n";
                finalSiftShip[row][columnLeft] = siftShip[i];
            columnLeft  -= 1 ;
            left = false;

            }
            
        }
        else{
            //td::cout << "row = "<< row << " , col right = " << columnRight << "\n";

            finalSiftShip[row][columnRight] = siftShip[i];
            columnRight +=1; 
            left = true;
        }
        if(columnRight == 12)
        {
            columnLeft = 5;
            columnRight = 6;
            left = true;
            row++;
        }
    }
    //printShip(finalSiftShip);
    return finalSiftShip;

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

bool equalShips(std::vector<std::vector<Container>>ship, std::vector<std::vector<Container>>SIFTship)
{
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 12; j++)
        {
            if(ship[i][j].stringWeight != SIFTship[i][j].stringWeight)
            {
                //std::cout << "i , j  = " << i << " , " << j <<  "\n";
                //std::cout << "weight of ship  = " << ship[i][j].stringWeight << " Weight of SIFTSHIP = " << SIFTship[i][j].stringWeight << "\n";
                return false;
            }
        }
    }
    return true;
}

std::string stringify(std::vector<std::vector<Container>>&ship,int x, int y)
{

    std::string myString = "";
    for(int i = 0; i < ship.size(); i++)
    {
        for(int j = 0; j < ship[0].size(); j++)
        {
            myString += ship[i][j].contents;
        }
    }
    myString += std::to_string(x) + std::to_string(y);
    return myString;
}

std::vector<std::vector<Container>> SIFTSHIP;
std::string stringifyWeight(std::vector<std::vector<Container>>&ship)
{

    std::string myString = "";
    for(int i = 0; i < ship.size(); i++)
    {
        for(int j = 0; j < ship[0].size(); j++)
        {
            myString += std::to_string(ship[i][j].weight);
        }
    }
    return myString;
}

bool balancePossible = false;
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
                //std::cout << ship[i][j].contents << "\n";
                containerList.push_back(ship[i][j]);
                sum += ship[i][j].weight;
            }
            
        }
        
    }
    //std::cout << containerList.size() << "\n";
    //std::cout << sum << "\n";
    //devolp the particion for the answers
    
    
    //misplaced tiles
    
    Node currNode;
    currNode.currShip = ship;
    currNode.cost = 0;
    currNode.craneX = 8;
    currNode.craneY = 0;
    currNode.depth = 0;
    currNode.emptyCrane = true;
    currNode.instructions = "";
    currNode.rawInsructions = "";
    //Node* prev = *t;
    //printShip(currNode->currShip);
    //  
    SIFTSHIP = GenerateSIFT(ship);
    std::string stringSIFT = stringifyWeight(SIFTSHIP);

    std::priority_queue<Node, std::vector<Node>, myComparator>pqBalance;

    std::vector<int>left;
    left.push_back(3044);
    //left.push_back(1100);
    left.push_back(2020);
    //left.push_back(10000);
    left.push_back(2011);
    left.push_back(2007);
    //left.push_back(100);
    left.push_back(2000);

    std::sort(left.begin(), left.end());
    std::vector<int>right;
    right.push_back(1100);
    right.push_back(10000);

    currNode.balance = isBalanced(currNode.currShip, left, right, stringSIFT);
    pqBalance.push(currNode);
    std::unordered_set<std::string> myUOSetBalance;
    std::string myString = stringify(currNode.currShip,currNode.craneX,currNode.craneY);
    myUOSetBalance.insert(myString);
    
    long long nodesExpanded = 0;
    int i = 19;
    //printShip(ship);

    //std::cout << "THIS SHIP THIS SEEING IF IT CAN BE BALACNED" << std::endl;
    balancePossible = canBeBalanced(ship);

    while (pqBalance.size() != 0)
    {
        //break;
        //std::cout << "HERE " << "\n";
        
        //if it is the goal state return true;
        currNode = pqBalance.top();
        //std::cout << "HERE 2" << "\n";
        pqBalance.pop();
        //printShip(currNode->currShip);
        //std::cout << "THE CURR NODE IS " << currNode->balance << "\n";

        if(currNode.balance == 0)
        {
            //printShip(currNode->currShip);
            //std::cout << "this is not balance  = " << isBalanced(currNode->currShip,left, right ) << "\n\n\n\n";

            std::cout << "SOLUTION FOUND!" << "\n";
            break;
        }
        //std::cout << "SOLUTION FOUND!" << "\n";
        updateQueueBalance(pqBalance, currNode, left, right, myUOSetBalance, stringSIFT);

        //delete currNode;
        //std::cout << "HERE" << "\n";

        nodesExpanded++;
        //std::cout << "NODES EXPANDED: " << nodesExpanded << "\n";
        if(nodesExpanded % 5000 == 0)
        {
            std::cout << "NODES EXPANDED: " << nodesExpanded << "\n";
            std::cout << "pqBalance.size() = " << pqBalance.size() <<  "\n";
            std::cout << "depth = " <<currNode.depth << "\n";
            std::cout << "stringified weight = " << stringifyWeight( currNode.currShip) << "\n";
            std::cout << "should be  = " << stringSIFT << "\n";
            printShip(currNode.currShip);

            //std::cout << "DEPTH \n\n" << currNode->depth << "\n\n\n";
            //printInstructions(currNode);
        }
        //std::cout << "Manhat = " << currNode.cost << "\n";
        
        
        //std::cout << "pqBalance.size() = " << pqBalance.size() <<  "\n";
        //i--;
    }
    printShip(currNode.currShip);
    std::cout<< currNode.instructions; 
    std::cout << "cost?  " << currNode.depth << "\n";
    std::ofstream myfile;
    myfile.open ("instructions.txt");
    myfile << currNode.rawInsructions;
    myfile.close();
    myfile.open("instructionsText.txt");
    myfile << currNode.instructions;
    myfile.close();
    //int h = 0;
    //std::cout << "h = " << h << "\n";
    /*Node * temp = new Node();
    //Node * temp2 = new Node();
    temp = currNode;
    int h = 0;
    while(pqBalance.size() > 0)
    {
        temp = pqBalance.top();
        pqBalance.pop();
        delete temp;
        //std::cout << "pqBalance.size() = " << pqBalance.size() <<  "\n";
        //std::cout << "h = " << h << "\n";
        h++;
    }
    std::cout << "pqBalance.size() = " << pqBalance.size() <<  "\n";
    delete currNode;
    */


    std::cout << "NODES EXPANDED: " << nodesExpanded << "\n";
   

}
//12*12 possible moves it could make 
int mannhatanDistancePath(int x1, int y1, int x2, int y2)
{
    return std::abs(double(x1)-x2) + std::abs(double(y1)-y2);
    

}



int findPath(int x1, int y1, int x2, int y2,  std::vector<std::vector<Container>> ship)
{
    
    int cost = 0;
    std::priority_queue<Node, std::vector<Node>, myComparator>pqPath;
    //std::cout <<"findPath Called\n";
    Node currNode;
    currNode.cost = 0;
    int nodesExpanded = 0;
    currNode.currX = x1;
    currNode.currY = y1;
    currNode.depth = 0;
    pqPath.push(currNode);
    int currDepth = 0;
    std::unordered_set<std::pair<int,int>,hashFunctionPair>myUOSet;
    std::pair<int, int>myPair = std::make_pair(x1,y1);
    //delete currNode;
    myUOSet.insert(myPair);
    //delete currNode;
    //return 1;
    //mySet.insert(currNode);
    /*for(int i = 0; i < 1000; i++)
    {
        Node * hi = new Node();
        hi->currShip = ship;
        hi->cost = i;
        pqPath.push(hi);
    }

    Node * temp = new Node();
    while(pqPath.size() > 0)
    {
        temp = pqPath.top();
        pqPath.pop();
        delete temp;
        //std::cout << "pqBalance.size() = " << pqBalance.size() <<  "\n";
        //std::cout << "h = " << h << "\n";
    }
    
    currNode = new Node;
    currNode->cost = 0;
    //int nodesExpanded = 0;
    currNode->currX = x1;
    currNode->currY = y1;
    currNode->depth = 0;
    pqPath.push(currNode);
    */
    //currNode->currShip = ship;
    int finalCost = 0;
    int delete1 = 0;

    while(pqPath.size() != 0)
    {
    
        currNode = pqPath.top();
        pqPath.pop();
        nodesExpanded++;
        if(currNode.currX == x2 && currNode.currY == y2)
        {
            //std::cout << "Solution Found\n";
            finalCost = currNode.cost;
            //std::cout << "finalCost = " << finalCost << "\n";
            //delete currNode;
            break;
        }

        //need to create seen hash for less repeated states
        updateQueue(pqPath, currNode, x2, y2, ship, myUOSet);

        delete1++;




    }

    
    return finalCost;

    //return finalCost;

    


     
}

void updateQueue(std::priority_queue<Node, std::vector<Node>, myComparator>& pq, Node currNode, int x2, int y2, std::vector<std::vector<Container>> &ship, std::unordered_set<std::pair<int,int>,hashFunctionPair> &myUOset )
{
    //time to explore the different nodes
    
    //std::vector<std::vector<Container>> ship = currNode->currShip;
    int x = currNode.currX;
    int y = currNode.currY;
    //x, y to move down
    //oops x and y axis flipped
    int created = 0;
    
    if(x != 0)
    {
        std::pair<int, int>myPair = std::make_pair(x-1,y);
        if((ship[(x - 1)][y].isEmpty && myUOset.find(myPair) == myUOset.end()) || ((x-1) == x2 && y == y2))
        {    
            Node leftNode;
            created++;
            //leftNode->prev = currNode;
            leftNode.currX = x-1;
            leftNode.currY = y;  
            leftNode.depth = currNode.depth+1;
            leftNode.cost = currNode.depth  + 1+ mannhatanDistancePath((x - 1), y, x2, y2);
            //std::cout<< myPair.first << ", " << myPair.second << "\n";            
            myUOset.insert(myPair);
            pq.push(leftNode);
        }
    }
    //x, y to move up
    if(x != 9)
    {
        std::pair<int, int>myPair = std::make_pair(x+1,y);
        if((ship[(x + 1)][y].isEmpty && myUOset.find(myPair) == myUOset.end())  || ((x+1) == x2 && y == y2))
        {
            Node rightNode;
            created++;
            //rightNode->prev = currNode;
            rightNode.currX = x+1; 
            rightNode.currY = y;
            rightNode.depth = currNode.depth+1;
            rightNode.cost = currNode.depth  + 1+ mannhatanDistancePath((x + 1), y, x2, y2);
            //std::cout<< myPair.first << ", " << myPair.second << "\n";  
            myUOset.insert(myPair);
            pq.push(rightNode);
        }
    }
    //x, y to move left
    if(y != 0)
    {
        std::pair<int, int>myPair = std::make_pair(x,y-1);
        if((ship[x][y-1].isEmpty && myUOset.find(myPair) == myUOset.end() ) || (x == x2 && (y-1) == y2))
        {
            Node downNode;
            created++;
            //downNode->prev = currNode;
            downNode.currX = x;
            downNode.currY = y-1; 
            downNode.depth = currNode.depth+1;
            downNode.cost = currNode.depth + 1 + mannhatanDistancePath(x, (y-1), x2, y2);
            //std::cout<< myPair.first << ", " << myPair.second << "\n";  
            myUOset.insert(myPair);
            pq.push(downNode);
        }
    }
    //x, y to move right
    if(y != 11)
    {
        std::pair<int, int>myPair = std::make_pair(x,y+1);
        if((ship[x][y+1].isEmpty && myUOset.find(myPair) == myUOset.end()) || (x == x2 && (y+1) == y2))
        {
            Node upNode;
            created++;
            //upNode->prev = currNode;
            upNode.currX = x;
            upNode.currY = y+1; 
            upNode.depth = currNode.depth+1;
            upNode.cost = currNode.depth + 1 + mannhatanDistancePath(x, (y+1), x2, y2);
            //std::cout<< myPair.first << ", " << myPair.second << "\n";  
            myUOset.insert(myPair);
            pq.push(upNode);
        }
    }
    //std::cout << "Created = " << created << "\n";


}

void printPath(Node * currNode)
{

    /*while(currNode != NULL)
    {
        std::cout << currNode->currX << ", " << currNode->currY <<" With a cost of " << currNode->cost << "\n";
        currNode = currNode->prev;
    }
    */
   std::cout  << "UNDER CONSTRUCTION PRINTPATH\n";  
}



std::vector<std::pair<int,int>> findContianersToMove(std::vector<std::vector<Container>>ship)
{
    //std::cout <<"STARTED TO FIND CONTAINERS\n";
    //printShip(ship);
    std::vector<std::pair<int,int>>moves;
    for (int j = 0; j < 12; j++)
    {
        int i = 9;
        //std::cout << i << ", " << j << "\n";
        while(i >= 0)
        {

            if(ship[i][j].isContanier)
            {
                std::pair<int, int>myPair = std::make_pair(i,j);
                moves.push_back(myPair);
                //std::cout << i << ", " << j << "\n";
                break;
            }
            i--;
        }
        //std::cout <<" TO FIND CONTAINERS\n";

    }
    return moves;

}

std::vector<std::pair<int,int>> findOpenPlaces(std::vector<std::vector<Container>>ship)
{
    //std::cout <<"STARTED TO FIND CONTAINERS\n";
    //printShip(ship);
    std::vector<std::pair<int,int>>moves;
    for (int j = 0; j < 12; j++)
    {
        int i = 0;
        while(i <= 9)
        {

            if(ship[i][j].isEmpty)
            {
                std::pair<int, int>myPair = std::make_pair(i,j);
                moves.push_back(myPair);
                //std::cout << i << ", " << j << "\n";
                break;
            }
            i++;
        }
        

    }
    return moves;
}


void swapContainers( std::vector<std::vector<Container>>&ship, int x1, int y1, int x2, int y2)
{
    int tempX = ship[x1][y1].x;
    int tempY = ship[x1][y1].y;
    ship[x1][y1].x = ship[x2][y2].x;
    ship[x1][y1].y = ship[x2][y2].y;
    ship[x2][y2].x = tempX;
    ship[x2][y2].y = tempY;
    Container tempContainer = ship[x1][y1];
    ship[x1][y1] = ship[x2][y2];
    ship[x2][y2] = tempContainer;


}

void removeContainer( std::vector<std::vector<Container>>&ship, int x1,int y1)
{
    Container tempContainer;
    tempContainer.accessable = true;
    tempContainer.isContanier = false;
    tempContainer.isEmpty = true;
    tempContainer.contents = "UNUSED";
    tempContainer.x = ship[x1][y1].x;
    tempContainer.y = ship[x1][y1].y;
    tempContainer.stringWeight = "00000";
    tempContainer.weight = 0;
    ship[x1][y1] = tempContainer;
    


}

void addContainer(std::vector<std::vector<Container>>&ship,int x1,int y1, std::vector<Container>& load)
{

    Container tempContainer;
    tempContainer.accessable = true;
    tempContainer.isContanier = true;
    tempContainer.isEmpty = false;
    tempContainer.contents = load[load.size()-1].contents;
    tempContainer.x = x1+1;
    tempContainer.y = y1+1;
    tempContainer.stringWeight = load[load.size()-1].stringWeight;
    tempContainer.weight = load[load.size()-1].weight;
    ship[x1][y1] = tempContainer;
    load.pop_back();
    

}

std::vector<int> combination;
std::vector<std::vector<int>> finalCombo1;
void addVector(const std::vector<int>& v) {
    std::vector<int> myVec;
    static int count = 0;
    //cout << "combination no " << (++count) << ": [ ";
    for (int i = 0; i < v.size(); ++i) { 
        
        myVec.push_back(v[i]);
        //cout << v[i] << " "; 
        }
    //cout << "] " << endl;
    finalCombo1.push_back(myVec);
  //return myVec;
}

void calaculatePartitions(int offset, int k,  std::vector<int> allWeights ) {
  //std::cout << "MADE TO CALC PARTI k = " << k  << "\n";
  if (k == 0) {
      std::vector<int> combos;
    //cout << "pretty called" << endl;
        addVector(combination);
    //combos = pretty_print(combination);
    return;
    
  }
 
  for (int i = offset; i <= allWeights.size() - k; ++i) {
    combination.push_back(allWeights[i]);
    
    calaculatePartitions(i+1, k-1, allWeights);
    
    combination.pop_back();
  }
  //std::vector<int>finalCombos;

}


bool canBeBalanced(std::vector<std::vector<Container>>&ship)
{
    int leftSum = 0;
    int rightSum = 0;
    std::vector<int>allWeights;
    std::vector<Container>leftShip;
    std::vector<Container>rightShip;
    for( int i = 9; i >= 0; i--)
    {
        for (int j = 0; j < 12; j++)
        {
           if(ship[i][j].isEmpty || !ship[i][j].accessable)
           {
               continue;
           }
           if(j <= 5)
           {
               leftShip.push_back(ship[i][j]);
               leftSum += ship[i][j].weight;
               allWeights.push_back(ship[i][j].weight);
               //std::cout << "left ship " << ship[i][j].weight << " "<< i << " , " <<  j << "\n";
           }
           else
           {
               allWeights.push_back(ship[i][j].weight);
               rightShip.push_back(ship[i][j]);
               rightSum += ship[i][j].weight;
               //std::cout << "right ship " << ship[i][j].weight << " "<< i << " , " <<  j << "\n";
           }
            
        }
    }
    //std::cout << "MADE IT TO HERE \n";

    for(int i = 0; i < allWeights.size()/2; i++)
    {
        //std::cout << "k = " << i << "\n";
         calaculatePartitions(0,i ,allWeights);
    }
    //std::cout << "MADE IT TO HERE2 \n";
    int totalSum = leftSum+rightSum;
    double allowance = (totalSum)*.05;
    //cout << "allowance = " << allowance << endl;
    double leftMin = (totalSum/2) - (allowance);
    double rightMax = (totalSum/2) + (allowance);
    std::vector<std::vector<int>> validArrangements;
    for(int i = 0; i < finalCombo1.size(); i++)
    {
        int sum = 0;
        for(int j = 0; j < finalCombo1[i].size(); j++)
        {
            sum += finalCombo1[i][j];
            //cout << finalCombo1[i][j] << " ";
        }
        sum = abs(double(sum)- totalSum);
        if (sum >= leftMin && sum <= rightMax)
        {
            return true;
            validArrangements.push_back(finalCombo1[i]);
            
        }
        //cout << endl;
        
    }
    if(validArrangements.size() == 0)
    {
        std::cout << "THIS SHIP CANNOT BE BALANCED\n";
        return false;
    }
    for(int i = 0; i < validArrangements.size(); i++)
    {
        //int sum = 0;
        for(int j = 0; j < validArrangements[i].size(); j++)
        {
            //sum += finalCombo1[i][j];
            std::cout << validArrangements[i][j] << " ";
        }
        
        std::cout << std::endl;
        
    }


}


int getDistanceFromSIFT(std::vector<std::vector<Container>>& ship)
{
    int sum = 0;
    std::vector<Container>myContainers;
    for(int i = 0; i < ship.size(); i++)
    {
        for(int j = 0; j < ship[0].size();j++)
        {
            if(ship[i][j].isContanier && (ship[i][j].contents != SIFTSHIP[i][j].contents))
            {
                myContainers.push_back(ship[i][j]);
                //sum++;
            }
        }
    }
    for(int i = 0 ; i < myContainers.size(); i++)
    {
        for(int j = 0; j < ship.size();j++)
        {
            bool found = false;;
           for(int k = 0; k < ship[0].size();k++)
            {
                if(myContainers[i].contents == SIFTSHIP[j][k].contents)
                {
                    sum += mannhatanDistancePath(myContainers[i].x,myContainers[i].y, j, k);
                    found = true;
                    break;
                }
            }
            if(found)
            {
                break;
            }
        }

    }

    return sum;
}

int isBalanced(std::vector<std::vector<Container>>& ship, std::vector<int>left, std::vector<int>right, std::string shipSIFT)
{

    int leftSum = 0;
    int rightSum = 0;
    std::vector<Container>leftShip;
    std::vector<Container>rightShip;
    if(!balancePossible)
    {
        int siftClose = getDistanceFromSIFT(ship);
        return siftClose;


    }
    for( int i = 9; i >= 0; i--)
    {
        for (int j = 0; j < 12; j++)
        {
           if(ship[i][j].isEmpty || !ship[i][j].accessable)
           {
               continue;
           }
           if(j <= 5)
           {
               leftShip.push_back(ship[i][j]);
               leftSum += ship[i][j].weight;
               //std::cout << "left ship " << ship[i][j].weight << " "<< i << " , " <<  j << "\n";
           }
           else
           {
               rightShip.push_back(ship[i][j]);
               rightSum += ship[i][j].weight;
               //std::cout << "right ship " << ship[i][j].weight << " "<< i << " , " <<  j << "\n";
           }
            
        }
    }



    int totalSum = rightSum+leftSum;
    
    double diff = abs(double(rightSum) - leftSum);

    //std::cout << "diff = " << diff << "\n";
    double percent = 15;
    if(rightSum >= leftSum)
    {
         percent = ((diff/double(rightSum)) * 100);
    }
    else
    {
         percent = ((diff/double(leftSum)) * 100);
    }
    double allowance = totalSum*.1;

    if(stringifyWeight(ship) == shipSIFT)
    {
        return 0;
    }

    //double percent = ((diff/double(totalSum)) * 100);
    //std::cout << "PERCENT = " << percent << "\n";
    if(percent <= 10)
    {
        return 0;
    }
    else
    {
        //std::cout << "TOTAL SUM = " << totalSum << "\n";
        //std::cout << "DIFF = " << diff << "\n";
        
        //std::cout << "LEFTSUM = " << leftSum << "\n";
        
        

        int manhat = 0;
        sort( leftShip.begin( ), leftShip.end( ), [ ]( const Container lhs, const Container rhs )
        {
            return lhs.weight > rhs.weight;
        });
        //std::cout << "LEFTSUM = " << leftSum << "\n";
        for(int i = 0; i < leftShip.size(); i++)
        {
           // std::cout << leftShip.at(i).weight << "\n";

        }
         //std::cout << "RIGHTSUM  = " << rightSum << "\n";
        for(int i = 0; i < rightShip.size(); i++)
        {
            //std::cout << rightShip.at(i).weight << "\n";

        }
        sort( rightShip.begin( ), rightShip.end( ), [ ]( const Container lhs, const Container rhs )
        {
            return lhs.weight > rhs.weight;
        });

        if(rightSum <= leftSum)
        {
            for(int i = 0; i < leftShip.size(); i++)
            {
                //std::cout << "leftShip = " << leftShip.at(i).weight << "\n";
                if(leftShip.at(i).weight <= diff)
                {
                    diff -=(2 *leftShip.at(i).weight);
                    rightSum += leftShip.at(i).weight;
                    leftSum -= leftShip.at(i).weight;
                    manhat += mannhatanDistancePath(leftShip.at(i).x,leftShip.at(i).y,leftShip.at(i).x, 5 );
                } 
            }
        }
        else{
            for(int i = 0; i < rightShip.size(); i++)
            {
                if(rightShip.at(i).weight <= diff)
                {
                    diff -=(2 * rightShip.at(i).weight);
                    rightSum -= rightShip.at(i).weight;
                    leftSum += rightShip.at(i).weight;

                    manhat += mannhatanDistancePath(rightShip.at(i).x,rightShip.at(i).y,rightShip.at(i).x, 6 );
                } 
            }

        }
        
        if(manhat == 0)
        {
            return 1;
        }
        //eturn 1;
        //std::cout << manhat << "\n";
        return manhat;
    }

    return 1;

}


void printInstructions(Node * currNode)
{
    std::vector<int>left;
    left.push_back(3044);
    //left.push_back(1100);
    left.push_back(2020);
    //left.push_back(10000);
    left.push_back(2011);
    left.push_back(2007);
    //left.push_back(100);
    left.push_back(2000);

    std::sort(left.begin(), left.end());
    std::vector<int>right;
    right.push_back(1100);
    right.push_back(10000);
    //std::cout << "this is not balance  = " << isBalanced(currNode->currShip,left, right ) << "\n\n\n\n";
    std::vector<std::string>myVec;
    while(currNode != NULL)
    {
        myVec.push_back(currNode->instructions);
        //std::cout << currNode->instructions << "\n";
        //printShip(currNode->currShip);
        //currNode = currNode->prev;
    }
    //std::cout << myVec.size() << " her is size\n"; 
    for(int i = myVec.size()-1; i >= 0; i--)
    {
        //std::cout << i << " this is iher is size\n"; 
        std::cout << myVec[i];
    }
}
void updateQueueBalance(std::priority_queue<Node, std::vector<Node>, myComparator>& pqBalance, Node  currNode, std::vector<int>&left, std::vector<int>&right, std::unordered_set<std::string>& myUOSetBalance,  std::string shipSIFT)
{
    //std::cout << "GETTING CONTAINERS\n";
    std::vector<std::vector<Container>> ship = currNode.currShip;
    std::vector<std::pair<int,int>>containerToAccess =  findContianersToMove(ship);
    std::vector<std::pair<int,int>>openSpacesToAccess =  findOpenPlaces(ship);

    int craneX = currNode.craneX;
    int craneY = currNode.craneY;
    int nodesAdded = 0;
    if(currNode.emptyCrane == true)
    {
        for( int i = 0; i < containerToAccess.size(); i++)
        {
            std::pair<int,int>currContianerPair = containerToAccess[i];
            //COST TO GET TO THE CONTAINER
            int costToGetToContainer = findPath(craneX, craneY, currContianerPair.first, currContianerPair.second, ship);
            Node newNode;
            std::string craneXString = std::to_string(craneX+1);
            std::string craneYString = std::to_string(craneY+1);
            if(craneX+1 < 10)
            {
                craneXString = "0"  + craneXString;
            } 
            if(craneY+1 < 10)
            {
                craneYString = "0" + craneYString;
            } 

            std::string currXString = std::to_string(currContianerPair.first+1);
            std::string currYString = std::to_string(currContianerPair.second+1);
            if(currContianerPair.first+1 < 10)
            {
                currXString = "0"  + currXString;
            } 
            if(currContianerPair.second+1 < 10)
            {
                currYString = "0" + currYString;
            } 
                
            
            newNode.currShip = ship;
            int balance = isBalanced(newNode.currShip,left,right, shipSIFT);
            newNode.emptyCrane = false;
            newNode.craneX = currContianerPair.first;
            newNode.craneY = currContianerPair.second;
            std::string tempString = stringify(newNode.currShip,newNode.craneX,newNode.craneY);
            if (myUOSetBalance.find(tempString)  == myUOSetBalance.end() )
            {
                newNode.depth = currNode.depth + costToGetToContainer;
                newNode.cost =currNode.depth + 1+ balance + costToGetToContainer;
                newNode.instructions = currNode.instructions + "Move Crane from (" + std::to_string(craneX) + ", " +  std::to_string(craneY)  + ") to (" +  std::to_string(currContianerPair.first) + " , " 
                    +  std::to_string(currContianerPair.second)+ ")\n";
                newNode.rawInsructions = currNode.rawInsructions + "[" + craneXString + "," + craneYString  + "] [" +  currXString + "," 
                    +  currYString+ "] \n";
                pqBalance.push(newNode);
                myUOSetBalance.insert(tempString);
                nodesAdded++;
            }

            
        }

    }
    else
    {
        for(int j = 0; j < openSpacesToAccess.size(); j++)
        {
            
            std::pair<int,int>currEmptyPair = openSpacesToAccess[j];
            //Get cost of crane to this container 
            if (craneY != currEmptyPair.second)
            {
                //std::cout << findPath(currContianerPair.first, currContianerPair.second, currEmptyPair.first, currEmptyPair.second, ship) << "\n";
                int cost = findPath(craneX, craneY, currEmptyPair.first, currEmptyPair.second, ship);
                //std::cout << "FINDING WHERE TO PUT\n";
                Node newNode;
                //std::cout << "FINDING WHERE TO PUT\n";
                newNode.instructions = currNode.instructions + "Move ("+  std::to_string(craneX) + " , "+  std::to_string(craneY)+ ") to ("
                 +  std::to_string(currEmptyPair.first) + " , " +  std::to_string(currEmptyPair.second) + ")\n";

                std::string craneXString = std::to_string(craneX+1);
                std::string craneYString = std::to_string(craneY+1);
                if(craneX+1 < 10)
                {
                    craneXString = "0"  + craneXString;
                } 
                if(craneY+1 < 10)
                {
                    craneYString = "0" + craneYString;
                } 

                std::string currXString = std::to_string(currEmptyPair.first+1);
                std::string currYString = std::to_string(currEmptyPair.second+1);
                if(currEmptyPair.first+1 < 10)
                {
                    currXString = "0"  + currXString;
                } 
                if(currEmptyPair.second+1 < 10)
                {
                    currYString = "0" + currYString;
                } 
                
                
                newNode.rawInsructions = currNode.rawInsructions + "[" + craneXString + "," + craneYString +"] ["  +  currXString + "," + currYString + "]\n";
                //newNode->prev = currNode;
                //leftNode->currX = x;
                //leftNode->currY = y;  
                //std::cout << "COST  : " << cost << "\n";
                newNode.currShip = ship;
                swapContainers(newNode.currShip,craneX, craneY, currEmptyPair.first,currEmptyPair.second);
                std::string tempString = stringify(newNode.currShip,newNode.craneX,newNode.craneY);
                if (myUOSetBalance.find(tempString)  == myUOSetBalance.end() )
                {
                    int balance = isBalanced(newNode.currShip,left,right,shipSIFT);
                    if(balance == 0)
                    {
                        //std::cout << "RIGHT HEREEEE\n";
                        //printShip(newNode->currShip);
                    }
                    newNode.craneX = currEmptyPair.first;
                    newNode.craneY = currEmptyPair.second;
                    newNode.balance = balance;
                    newNode.emptyCrane = true;
                    //std::cout << "NEW NODE balance  : " << newNode.balance << "\n";
                    newNode.depth = currNode.depth + cost;
                    //std::cout << "NEW NODE depth  : " << newNode.depth << "\n";
                    newNode.cost = currNode.depth + 1+ balance + cost;
                    //std::cout<< myPair.first << ", " << myPair.second << "\n";            
                    myUOSetBalance.insert(tempString);
                    //std::cout << "NEW NODE COST  : " << newNode.cost << "\n";
                    pqBalance.push(newNode);
                    nodesAdded++;
                }
                
                /*
                newNode->craneX = currEmptyPair.first;
                newNode->craneY = currEmptyPair.second;
                newNode->balance = balance;
                newNode->depth = currNode->depth+1;
                newNode->cost = currNode->depth + 1+ balance;
                //std::cout<< myPair.first << ", " << myPair.second << "\n";            
                //myUOset.insert(myPair);
                pqBalance.push(newNode);
                */
                //std::cout << "pqBalance.size() = " << pqBalance.size() <<  "\n";

                //std::cout<<"COST FROM " <<  currContianerPair.first << ", " << currContianerPair.second << " TO " << currEmptyPair.first << ", " << currEmptyPair.second << " is " << cost << "\n";
                
                //Make a node with the changes in it?
                //update the cost of the node
                //keep the location of the crane             
            }
        }
    }
    /*
    for(int i = 0; i < containerToAccess.size(); i++)
    {

        std::pair<int,int>currContianerPair = containerToAccess[i];
        //COST TO GET TO THE CONTAINER
        int costToGetToContainer = findPath(craneX, craneY, currContianerPair.first, currContianerPair.second, ship);
        
        
        //std::cout << "COST TO GET TO CONATINER : " << costToGetToContainer << "\n";
        for(int j = 0; j < openSpacesToAccess.size(); j++)
        {
            std::pair<int,int>currEmptyPair = openSpacesToAccess[j];
            //Get cost of crane to this container 
            if (currContianerPair.second != currEmptyPair.second)
            {
                //std::cout << findPath(currContianerPair.first, currContianerPair.second, currEmptyPair.first, currEmptyPair.second, ship) << "\n";
                int cost = costToGetToContainer + findPath(currContianerPair.first, currContianerPair.second, currEmptyPair.first, currEmptyPair.second, ship);
                //std::cout << "FINDING WHERE TO PUT\n";
                Node newNode;
                //std::cout << "FINDING WHERE TO PUT\n";
                newNode.instructions = "Move from Crane (" + std::to_string(craneX) + ", " +  std::to_string(craneY)  + ") to (" +  std::to_string(currContianerPair.first) + " , " 
                +  std::to_string(currContianerPair.second)+ ") \nMove ("+  std::to_string(currContianerPair.first) + " , "+  std::to_string(currContianerPair.second)+ ") to ("
                 +  std::to_string(currEmptyPair.first) + " , " +  std::to_string(currEmptyPair.second) + ") \n weight = " + std::to_string(ship[currContianerPair.first][currContianerPair.second].weight) + "cost = "  + std::to_string(cost) + "\n\n";
                //newNode->prev = currNode;
                //leftNode->currX = x;
                //leftNode->currY = y;  
                //std::cout << "COST  : " << cost << "\n";
                newNode.currShip = ship;
                swapContainers(newNode.currShip,currContianerPair.first, currContianerPair.second, currEmptyPair.first,currEmptyPair.second);
                std::string tempString = stringify(newNode.currShip);
                if (myUOSetBalance.find(tempString)  == myUOSetBalance.end() )
                {
                    int balance = isBalanced(newNode.currShip,left,right);
                    if(balance == 0)
                    {
                        //std::cout << "RIGHT HEREEEE\n";
                        //printShip(newNode->currShip);
                    }
                    newNode.craneX = currEmptyPair.first;
                    newNode.craneY = currEmptyPair.second;
                    newNode.balance = balance;
                    //std::cout << "NEW NODE balance  : " << newNode.balance << "\n";
                    newNode.depth = currNode.depth + cost;
                    //std::cout << "NEW NODE depth  : " << newNode.depth << "\n";
                    newNode.cost = currNode.depth + 1+ balance + cost;
                    //std::cout<< myPair.first << ", " << myPair.second << "\n";            
                    myUOSetBalance.insert(tempString);
                    //std::cout << "NEW NODE COST  : " << newNode.cost << "\n";
                    pqBalance.push(newNode);
                    nodesAdded++;
                }
                
                
                newNode->craneX = currEmptyPair.first;
                newNode->craneY = currEmptyPair.second;
                newNode->balance = balance;
                newNode->depth = currNode->depth+1;
                newNode->cost = currNode->depth + 1+ balance;
                //std::cout<< myPair.first << ", " << myPair.second << "\n";            
                //myUOset.insert(myPair);
                pqBalance.push(newNode);
                
                //std::cout << "pqBalance.size() = " << pqBalance.size() <<  "\n";

                //std::cout<<"COST FROM " <<  currContianerPair.first << ", " << currContianerPair.second << " TO " << currEmptyPair.first << ", " << currEmptyPair.second << " is " << cost << "\n";
                
                //Make a node with the changes in it?
                //update the cost of the node
                //keep the location of the crane             
            }

        }   
    }*/
    //std::cout << "NODES ADDED = "<< nodesAdded << "\n";

    
    
    //std::cout << "HERE" << "\n";



}
    