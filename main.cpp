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
    int operator() (const Node* n1, const Node* n2)
    {
        return n1->cost > n2->cost;
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
void updateQueue(std::priority_queue<Node*, std::vector<Node*>, myComparator>&, Node *, int , int, std::vector<std::vector<Container>>&, std::unordered_set<std::pair<int,int>,hashFunctionPair>&);
int findPath(int, int, int, int,  std::vector<std::vector<Container>> );
int mannhatanDistancePath(int, int, int, int);
void printPath(Node *);
std::vector<std::pair<int,int>> findContianersToMove(std::vector<std::vector<Container>>);
std::vector<std::pair<int,int>> findOpenPlaces(std::vector<std::vector<Container>>);
void swapContainers( std::vector<std::vector<Container>>&, int, int, int, int);
void updateQueueBalance(std::priority_queue<Node*, std::vector<Node*>, myComparator>& , Node * ,std::vector<int>&, std::vector<int>&,  std::unordered_set<std::string>&);
int isBalanced(std::vector<std::vector<Container>>& , std::vector<int>, std::vector<int>);
std::string stringify(std::vector<std::vector<Container>>&);
void printInstructions(Node * );



int main()
{
    //std::cout << "started\n";
    std::vector<std::vector<Container>>ship = readinFile();
    //containersToBeLoaded();
    //containersToBeUnloaded();
    //std::cout << "done\n";
    printShip(ship);
    balance(ship);
    //std::cout << ship.size() << "\n";
    //std::cout << ship[0].size() << "\n";
    //std::cout << findPath( 8,  0,  1,  1, ship) << "\n";
    //findContianersToMove(ship);
    //findOpenPlaces(ship);
    //swapContainers(ship,4,4,1,1 );
    //printShip(ship);
    //std::cout << findPath( 0,  1,  0,  6, ship);
    //Read in the ship contents
    //testing(ship);
    // Get what needs to come off / on
    
    // queue of things that needs to get on


    //balance 
    //balance(ship);
    //std::cout << mannhatanDistancePath(1,1,6,6);
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



std::string stringify(std::vector<std::vector<Container>>&ship)
{

    std::string myString = "";
    for(int i = 0; i < ship.size(); i++)
    {
        for(int j = 0; j < ship[0].size(); j++)
        {
            myString += ship[i][j].contents;
        }
    }
    return myString;
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
    
    Node* currNode = new Node();
    currNode->currShip = ship;
    currNode->cost = 0;
    currNode->craneX = 8;
    currNode->craneY = 0;
    //Node* prev = *t;
    //printShip(currNode->currShip);
    //  
    std::priority_queue<Node*, std::vector<Node*>, myComparator>pqBalance;
    //pqBalance.push(currNode);
    //Node* currNode1 = new Node();
    //currNode1->currShip = ship;
    //currNode1->cost = 100;
    //pq.push(currNode1);
    //Node* p = pq.top();
    //std::cout << p->cost << "\n";
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
    currNode->balance = isBalanced(currNode->currShip, left, right);
    /*std::cout << "THE CURR NODE IS " << currNode->balance << "\n";
    swapContainers(ship,7 , 4, 1, 1);
    printShip(ship);
    std::cout << "THE CURR NODE IS " << isBalanced(ship, left, right) << "\n";
    swapContainers(ship,6 , 4, 1, 7);
    printShip(ship);
    std::cout << "THE CURR NODE IS " << isBalanced(ship, left, right) << "\n";
    swapContainers(ship,5 , 4, 2, 1);
    printShip(ship);
    std::cout << "THE CURR NODE IS " << isBalanced(ship, left, right) << "\n";
    swapContainers(ship,4 , 4, 2, 7);
    printShip(ship);
    std::cout << "THE CURR NODE IS " << isBalanced(ship, left, right) << "\n";
    */

    pqBalance.push(currNode);
    //std::unordered_set<std::vector<std::vector<Container>>,hashFunctionNode>myUOSetBalance;
    std::unordered_set<std::string> myUOSetBalance;
    std::string myString = stringify(currNode->currShip);
    //std::cout<<"MY STRING : " << myString << "\n";
    myUOSetBalance.insert(myString);
    //hashFunctionNode
    /*
    std::unordered_set<std::pair<int,int>,hashFunctionPair>myUOSet;
    std::pair<int, int>myPair = std::make_pair(x1,y1);
    myUOSet.insert(myPair);

    */
   
    long long nodesExpanded = 0;
    int i = 19;
    //printShip(ship);
    while (pqBalance.size() != 0)
    {
        
        //std::cout << "HERE " << "\n";
        
        //if it is the goal state return true;
        currNode = pqBalance.top();
        //std::cout << "HERE 2" << "\n";
        pqBalance.pop();
        //printShip(currNode->currShip);
        //std::cout << "THE CURR NODE IS " << currNode->balance << "\n";

        if(currNode->balance == 0)
        {
            //printShip(currNode->currShip);
            //std::cout << "this is not balance  = " << isBalanced(currNode->currShip,left, right ) << "\n\n\n\n";
            std::cout << "SOLUTION FOUND!" << "\n";
            break;
        }
        updateQueueBalance(pqBalance, currNode, left, right, myUOSetBalance);
        //std::cout << "HERE" << "\n";

        nodesExpanded++;
        //std::cout << "NODES EXPANDED: " << nodesExpanded << "\n";
        
        //std::cout << "pqBalance.size() = " << pqBalance.size() <<  "\n";
        //i--;
    }
    printShip(currNode->currShip);
    printInstructions(currNode);
    std::cout << "cost?  " << currNode->depth << "\n";

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
    std::unordered_set<std::pair<int,int>,hashFunctionPair>myUOSet;
    std::pair<int, int>myPair = std::make_pair(x1,y1);
    myUOSet.insert(myPair);

    //mySet.insert(currNode);
    while(pqPath.size() != 0)
    {
        currNode = pqPath.top();
        pqPath.pop();
        nodesExpanded++;
        if(currNode->currX == x2 && currNode->currY == y2)
        {
            //std::cout << "Solution Found\n";
            break;
        }

        //need to create seen hash for less repeated states
        updateQueue(pqPath, currNode, x2, y2, ship, myUOSet);
        //currDepth++;
        //std::cout << nodesExpanded << "\n";



    }
    //std::cout << nodesExpanded << "\n";
    //printPath(currNode);
    //std::cout << "size of ship 1 axis " << ship.size() << "\n";
    //std::cout << "size of ship 2 axis " << ship[0].size() << "\n";
    //std::cout << "size of myUO " << myUOSet.size() << "\n";
    //std::cout << "max size of myUO " << myUOSet.max_size() << "\n";

    return currNode->cost;

    


     
}

void updateQueue(std::priority_queue<Node*, std::vector<Node*>, myComparator>& pq, Node * currNode, int x2, int y2, std::vector<std::vector<Container>> &ship, std::unordered_set<std::pair<int,int>,hashFunctionPair> &myUOset )
{
    //time to explore the different nodes
    
    //std::vector<std::vector<Container>> ship = currNode->currShip;
    int x = currNode->currX;
    int y = currNode->currY;
    //x, y to move down
    //oops x and y axis flipped
    
    if(x != 0)
    {
        std::pair<int, int>myPair = std::make_pair(x-1,y);
        if((ship[(x - 1)][y].isEmpty && myUOset.find(myPair) == myUOset.end()) || ((x-1) == x2 && y == y2))
        {    
            Node* leftNode = new Node();
            leftNode->prev = currNode;
            leftNode->currX = x-1;
            leftNode->currY = y;  
            leftNode->depth = currNode->depth+1;
            leftNode->cost = currNode->depth  + 1+ mannhatanDistancePath((x - 1), y, x2, y2);
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
            Node* rightNode = new Node();
            rightNode->prev = currNode;
            rightNode->currX = x+1; 
            rightNode->currY = y;
            rightNode->depth = currNode->depth+1;
            rightNode->cost = currNode->depth  + 1+ mannhatanDistancePath((x + 1), y, x2, y2);
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
            Node* downNode = new Node();
            downNode->prev = currNode;
            downNode->currX = x;
            downNode->currY = y-1; 
            downNode->depth = currNode->depth+1;
            downNode->cost = currNode->depth + 1 + mannhatanDistancePath(x, (y-1), x2, y2);
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
            Node* upNode = new Node();
            upNode->prev = currNode;
            upNode->currX = x;
            upNode->currY = y+1; 
            upNode->depth = currNode->depth+1;
            upNode->cost = currNode->depth + 1 + mannhatanDistancePath(x, (y+1), x2, y2);
            //std::cout<< myPair.first << ", " << myPair.second << "\n";  
            myUOset.insert(myPair);
            pq.push(upNode);
        }
    }


}

void printPath(Node * currNode)
{
    while(currNode != NULL)
    {
        std::cout << currNode->currX << ", " << currNode->currY <<" With a cost of " << currNode->cost << "\n";
        currNode = currNode->prev;
    }
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
    Container tempContainer = ship[x1][y1];
    ship[x1][y1] = ship[x2][y2];
    ship[x2][y2] = tempContainer;


}

int isBalanced(std::vector<std::vector<Container>>& ship, std::vector<int>left, std::vector<int>right)
{
    std::vector<int>leftShip;
    std::vector<int>rightShip;
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
               leftShip.push_back(ship[i][j].weight);
               //std::cout << "left ship " << ship[i][j].weight << " "<< i << " , " <<  j << "\n";
           }
           else
           {
               rightShip.push_back(ship[i][j].weight);
               //std::cout << "right ship " << ship[i][j].weight << " "<< i << " , " <<  j << "\n";
           }
            
        }
    }
    /*if(leftShip.size() != left.size() || rightShip.size() != right.size())
    {
        return (std::abs(double(leftShip.size()) - left.size()) + std::abs(double(rightShip.size()) - right.size()));
        std::cout << "FAILED HERE \n";
        //return false;
    }
    */

    //std::sort(leftShip.begin(), leftShip.end());
    //std::sort(rightShip.begin(), rightShip.end());
    int wrong = 0;
    bool found = false;
    bool startLeft = false;
    for(int i = 0; i < leftShip.size() ; i++)
    {
        for(int j = 0; j < left.size(); j++)
        {
            startLeft = true;
            if(leftShip[i] == left[j])
            {
                found = true;
            }
        }
        if(!found)
        {
            wrong++;
        }
        found = false;
        
    }
    bool startRight = false;
    for(int i = 0; i < rightShip.size() ; i++)
    {
        for(int j = 0; j < right.size(); j++)
        {
            startRight = true;
            if(rightShip[i] == right[j])
            {
                found = true;
            }
        }
        if(!found)
        {
            wrong++;
        }
        found = false;
        
    }
    if(!startRight)
    {
        wrong += rightShip.size() + right.size();
    }
    if(!startLeft)
    {
        wrong += leftShip.size() + left.size();
    }

    
    //std::cout << "FAILED HERE \n";

    return wrong;

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
        currNode = currNode->prev;
    }
    for(int i = myVec.size(); i >= 0; i--)
    {
        std::cout << myVec[i];
    }
    
}
void updateQueueBalance(std::priority_queue<Node*, std::vector<Node*>, myComparator>& pqBalance, Node * currNode, std::vector<int>&left, std::vector<int>&right, std::unordered_set<std::string>& myUOSetBalance)
{
    //std::cout << "GETTING CONTAINERS\n";
    std::vector<std::vector<Container>> ship = currNode->currShip;
    std::vector<std::pair<int,int>>containerToAccess =  findContianersToMove(ship);
    /*for(int i = 0; i < containerToAccess.size(); i++)
    {
        std::pair<int,int>currPair = containerToAccess[i];
        //std::cout<< currPair.first << ", " << currPair.second << "\n";

    }
    */
    std::vector<std::pair<int,int>>openSpacesToAccess =  findOpenPlaces(ship);
    /*std::cout << "GETTING SPACES\n";
    for(int i = 0; i < openSpacesToAccess.size(); i++)
    {
        std::pair<int,int>currPair = openSpacesToAccess[i];
        //std::cout<< currPair.first << ", " << currPair.second << "\n";

    }
    std::cout << "FINDING WHERE TO PUT\n";
    */ 
    //CRANE STARTS IN 8, 0
    int craneX = currNode->craneX;
    int craneY = currNode->craneY;
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
                int cost = costToGetToContainer + findPath(currContianerPair.first, currContianerPair.second, currEmptyPair.first, currEmptyPair.second, ship);
                
                Node* newNode = new Node();
                newNode->instructions = "Move from Crane (" + std::to_string(craneX) + ", " +  std::to_string(craneY)  + ") to (" +  std::to_string(currContianerPair.first) + " , " 
                +  std::to_string(currContianerPair.second)+ ") \nMove ("+  std::to_string(currContianerPair.first) + " , "+  std::to_string(currContianerPair.second)+ ") to ("
                 +  std::to_string(currEmptyPair.first) + " , " +  std::to_string(currEmptyPair.second) + ") \n weight = " + std::to_string(ship[currContianerPair.first][currContianerPair.second].weight) + "cost = "  + std::to_string(cost) + "\n\n";
                newNode->prev = currNode;
                //leftNode->currX = x;
                //leftNode->currY = y;  
                newNode->currShip = ship;
                swapContainers(newNode->currShip,currContianerPair.first, currContianerPair.second, currEmptyPair.first,currEmptyPair.second);
                std::string tempString = stringify(newNode->currShip);
                if (myUOSetBalance.find(tempString)  == myUOSetBalance.end() )
                {
                    int balance = isBalanced(newNode->currShip,left,right);
                    if(balance == 0)
                    {
                        //std::cout << "RIGHT HEREEEE\n";
                        //printShip(newNode->currShip);
                    }
                    newNode->craneX = currEmptyPair.first;
                    newNode->craneY = currEmptyPair.second;
                    newNode->balance = balance;
                    newNode->depth = currNode->depth + cost;
                    newNode->cost = currNode->depth + 1+ balance + cost;
                    //std::cout<< myPair.first << ", " << myPair.second << "\n";            
                    myUOSetBalance.insert(tempString);
                    pqBalance.push(newNode);
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

    
    
    //std::cout << "HERE" << "\n";



}
    