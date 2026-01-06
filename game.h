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
    map<string,TypTerr> stringToTerr;

    map<string, BuildingInfo> bazaDanych;

    public:
    Game();
    void run();
    void startTutorial();
    void commands();
    void prntBudynki();

    void loadGameData();
    void prntInfo(string cat);
    void prntCategories();
    void prntHelp();
    void prntRules();

    bool checkConfig();
    void saveConfig();

    void sprawdzLvlTerr();

};

#endif
