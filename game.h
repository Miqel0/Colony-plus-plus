#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <memory>
#include <string>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <map>

using namespace std;
#include "building.h"
#include "colony.h"
#include "enums.h"

#ifndef game_h
#define game_h

class Game{
    Colony kolonia;
    bool running;
    map<string,TypBudynku> stringToBudynku;
    map<string,TypDomy> stringToDomy;
    map<string,TypEnergy> stringToEnergy;
    map<string,TypFarm> stringToFarm;
    map<string,TypProducer> stringToProducer;


    public:
    Game();
    void run();
    void commands();


};

#endif
