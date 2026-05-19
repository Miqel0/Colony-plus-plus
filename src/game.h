#ifndef game_h
#define game_h

#include <map>
#include <string>

using namespace std;

#include "enums.h"
#include "colony.h"
#include "graphics.h"
#include "buildingFactory.h"

/**
 * @brief Klasa zawierająca w sobie ogólną logikę gry, ma w sobie: Colony kolonia, Graphics grafika, BuildingFactory fabryka, map<string, BuildingInfo> bazaDanych 
 * 
 */
class Game{

    public:
        Game();
        ~Game(){};
        
        //GRA
        vector<BuildingInfo> UIprntNewLvlTerr();
        BuildResult UIbuild(BuildingInfo info);
        NextResult UINextRound();
        DestroyResult UIZburz(string nazwa);
        
        
        void UIrun();
        void load();
        //PLIKI
        void loadGameData();
        bool checkConfig();
        void saveConfig();

    private:

        //Tworzenie elementów:
        Colony kolonia;
        Graphics grafika;
        BuildingFactory fabryka;
        map<string, BuildingInfo> bazaDanych;

};

#endif
