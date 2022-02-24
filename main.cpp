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
        bool accessable;
        bool isContanier;
        bool toBeRemoved; 

};

int readinFile();
void containersToBeLoaded();
void containersToBeUnloaded();





int main()
{
    std::cout << "started\n";
    //readinFile();
    containersToBeLoaded();
    containersToBeUnloaded();
    std::cout << "done\n";
    /*long long count = 0;

    for (long long i = 0; i < 10000000000; i++) {
    count = i;
    }
    std::cout << count;
    */
    //Read in the ship contents


    // Get what needs to come off / on

    // queue of things that needs to get on


    // 


    //create global ship view
    return 0;


}



int readinFile(){

    std::ifstream inFile;
    std::string filename = "test.txt";
    inFile.open(filename);
    std::vector<std::vector<Container>>ship;
    std::string tempString = "";
    int x = 0;
    int y = 0;
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
            }
            else
            {
                tempContainer.accessable = false;
                if (tempContainer.contents == "UNUSED")
                {
                    tempContainer.isContanier = false;
                }
                else
                {
                     tempContainer.isContanier = true;

                }
            }
            //std::cout <<  tempString.substr(10,5) << "\n";
            tempContainer.weight = std::stoi(tempString.substr(10,5));
            //std::cout <<  tempContainer.weight << "\n";
            
            tempStore.push_back(tempContainer);
            
            x++;
        }
        ship.push_back(tempStore);
        x = 0;

    }
    //cout <<"here";
    std::cout << ship.size() << std::endl;
    std::cout << ship[0].size() << std::endl;
    //STILL WANT TO ADD OTHER ROWS TO THE SHIP
    return 2;
    

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