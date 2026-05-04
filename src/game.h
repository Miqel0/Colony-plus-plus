#ifndef game_h
#define game_h

#include <map>
#include <string>

using namespace std;

#include "enums.h"
#include "colony.h"
#include "graphics.h"

class Game{

    public:
        Game();
        ~Game(){};
        
        //GRA
        void grafikaStart();
        void run();
        void startTutorial();
        void commands();
        void build(BuildingInfo info);

        void UIrun();
        
        //PLIKI
        void loadGameData();
        bool checkConfig();
        void saveConfig();

        //WYSWIETLANIE
        void prntInfo(string cat);
        void prntCategories();
        void prntHelp();
        void prntRules();
        void prntNewLvlTerr();

    private:
        bool running;
        
        //Tworzenie obiektu kolonia
        Colony kolonia;
        Graphics grafika;

        //Mapy do zamiany strignu na typ budynku
        map<string,TypBudynku> stringToBudynku;
        map<string,TypDomy> stringToDomy;
        map<string,TypEnergy> stringToEnergy;
        map<string,TypFarm> stringToFarm;
        map<string,TypProducer> stringToProducer;
        map<string,TypTerr> stringToTerr;
        map<string, BuildingInfo> bazaDanych;

};

#endif
