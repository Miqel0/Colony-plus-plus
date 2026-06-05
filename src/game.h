#ifndef game_h
#define game_h

#include <map>
#include <string>

using namespace std;

#include "utils.h"
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
        BuildResult UIbuild(BuildingInfo info,pair<int,int> kliknietePole);
        NextResult UINextRound();
        DestroyResult UIZburz(string nazwa);
        DestroyResult UIZburzID(int ID);
        
        void save(const string& nazwa_zapisu );
        void UIrun();
        void load(const string& nazwa_zapisu );
        //PLIKI
        void loadGameData();
        void loadConfig();
        void saveConfig();

        vector<string> pobierzZapisy();
        vector<string>& getZapisy();
        void setZapisy();

        string getOstatniZapis() const;
        void setOstatniZapis(string naz);
        void setNazwa(string nazwa);
    private:

        map<string, BuildingInfo> bazaDanych;
        Colony kolonia;
        BuildingFactory fabryka;
        Graphics grafika;

        vector <string> zapisy;
        string ostatni_zapis;
        

};

#endif
