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
void updateQueue(std::priority_queue<Node, std::vector<Node>, myComparator>&, Node , int , int, std::vector<std::vector<Container>>&, std::unordered_set<std::pair<int,int>,hashFunctionPair>&);
int findPath(int, int, int, int,  std::vector<std::vector<Container>> );
int mannhatanDistancePath(int, int, int, int);
void printPath(Node *);
std::vector<std::pair<int,int>> findContianersToMove(std::vector<std::vector<Container>>);
std::vector<std::pair<int,int>> findOpenPlaces(std::vector<std::vector<Container>>);
void swapContainers( std::vector<std::vector<Container>>&, int, int, int, int);
void updateQueueBalance(std::priority_queue<Node, std::vector<Node>, myComparator>& , Node  ,std::vector<int>&, std::vector<int>&,  std::unordered_set<std::string>&);
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
    int i = 0;
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    /*
    while(i < 1000000)
    {
        //std::cout <<"hi\n";
        findPath( 1,  0,  2,  1, ship);
         //std::cout << findPath( 8,  0,  1,  1, ship) << "\n";
         //std::cout << findPath( 8,  0,  1,  1, ship) << "\n";
         //std::cout << findPath( 8,  0,  1,  1, ship) << "\n";
         //std::cout << i << "  hi\n";
         i++;
       
    }
    std::cout << "done\n";
    */
    
    while(1){

    }
    
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
    
    Node currNode;
    currNode.currShip = ship;
    currNode.cost = 0;
    currNode.craneX = 8;
    currNode.craneY = 0;
    currNode.depth = 0;
    currNode.emptyCrane = true;
    currNode.instructions = "";
    //Node* prev = *t;
    //printShip(currNode->currShip);
    //  
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
    currNode.balance = isBalanced(currNode.currShip, left, right);
    pqBalance.push(currNode);
    std::unordered_set<std::string> myUOSetBalance;
    std::string myString = stringify(currNode.currShip);
    myUOSetBalance.insert(myString);
    
    long long nodesExpanded = 0;
    int i = 19;
    //printShip(ship);
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
        updateQueueBalance(pqBalance, currNode, left, right, myUOSetBalance);

        //delete currNode;
        //std::cout << "HERE" << "\n";

        nodesExpanded++;
        //std::cout << "NODES EXPANDED: " << nodesExpanded << "\n";
        if(nodesExpanded % 5000 == 0)
        {
            std::cout << "NODES EXPANDED: " << nodesExpanded << "\n";
            std::cout << "pqBalance.size() = " << pqBalance.size() <<  "\n";
            printShip(currNode.currShip);

            //std::cout << "DEPTH \n\n" << currNode->depth << "\n\n\n";
            //printInstructions(currNode);
        }
        
        
        //std::cout << "pqBalance.size() = " << pqBalance.size() <<  "\n";
        //i--;
    }
    printShip(currNode.currShip);
    std::cout<< currNode.instructions; 
    std::cout << "cost?  " << currNode.depth << "\n";
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
    //currNode->currShip = ship;
    //delete currNode;
    //return 1;
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
        //std::cout << "size of pqPath" << pqPath.size() << "\n";
        delete1++;
        //delete currNode;
        //currDepth++;
        //std::cout << nodesExpanded << "\n";



    }
    //std::cout << "deleted = " << delete1 << "\n";
    //std::cout << "size of pqPath" << pqPath.size() << "\n";
    //std::cout << nodesExpanded << "\n";
    //printPath(currNode);
    //std::cout << "size of ship 1 axis " << ship.size() << "\n";
    //std::cout << "size of ship 2 axis " << ship[0].size() << "\n";
    //std::cout << "size of myUO " << myUOSet.size() << "\n";
    //std::cout << "max size of myUO " << myUOSet.max_size() << "\n";
    //int finalCost = currNode->cost;
   
    /*Node * temp = new Node();
    //Node * temp2 = new Node();
    //temp = currNode;
    int h = 0;
    while(pqPath.size() > 0)
    {
        delete1++;
        temp = pqPath.top();
        pqPath.pop();
        delete temp;
        //std::cout << "pqBalance.size() = " << pqBalance.size() <<  "\n";
        std::cout << "h = " << h << "\n";
        h++;
    }
    //std::cout << "here\n"; 
    std::cout << "pqBalance.size() = " << pqPath.size() <<  "\n";
    */
    //delete temp2;
    //delete temp;
    
    //delete currNode;
    
    //delete currNode;
    //delete1++;
    //std::cout << "deleted = " << delete1 << "\n";

    
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
    Container tempContainer = ship[x1][y1];
    ship[x1][y1] = ship[x2][y2];
    ship[x2][y2] = tempContainer;


}

int isBalanced(std::vector<std::vector<Container>>& ship, std::vector<int>left, std::vector<int>right)
{

    int leftSum = 0;
    int rightSum = 0;
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
               leftSum += ship[i][j].weight;
               //std::cout << "left ship " << ship[i][j].weight << " "<< i << " , " <<  j << "\n";
           }
           else
           {
               rightShip.push_back(ship[i][j].weight);
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
    //double percent = ((diff/double(totalSum)) * 100);
    //std::cout << "PERCENT = " << percent << "\n";
    if(percent <= 10)
    {
        return 0;
    }
    else
    {
        
        //return (int(diff)%5)+1;
        return 1;
    }

    /*
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
    */
    
    //std::cout << "FAILED HERE \n";

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
void updateQueueBalance(std::priority_queue<Node, std::vector<Node>, myComparator>& pqBalance, Node  currNode, std::vector<int>&left, std::vector<int>&right, std::unordered_set<std::string>& myUOSetBalance)
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
            newNode.instructions = currNode.instructions + "Move from Crane (" + std::to_string(craneX) + ", " +  std::to_string(craneY)  + ") to (" +  std::to_string(currContianerPair.first) + " , " 
                +  std::to_string(currContianerPair.second)+ ") \n";
            newNode.currShip = ship;
            int balance = isBalanced(newNode.currShip,left,right);
            newNode.emptyCrane = false;
            newNode.craneX = currContianerPair.first;
            newNode.craneY = currContianerPair.second;
            newNode.depth = currNode.depth + costToGetToContainer;
            newNode.cost =currNode.depth + 1+ balance + costToGetToContainer;
            pqBalance.push(newNode);
            nodesAdded++;
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
                 +  std::to_string(currEmptyPair.first) + " , " +  std::to_string(currEmptyPair.second) + ") \n weight = " + std::to_string(ship[craneX][craneY].weight) + "cost = "  + std::to_string(cost) + "\n\n";
                //newNode->prev = currNode;
                //leftNode->currX = x;
                //leftNode->currY = y;  
                //std::cout << "COST  : " << cost << "\n";
                newNode.currShip = ship;
                swapContainers(newNode.currShip,craneX, craneY, currEmptyPair.first,currEmptyPair.second);
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
    