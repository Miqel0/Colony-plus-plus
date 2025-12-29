#ifndef game_h
#define game_h

#include <map>
#include <string>

using namespace std;

#include "enums.h"
#include "colony.h"

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
    void prntBudynki();


};

#endif
