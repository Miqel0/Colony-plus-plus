#ifndef colony_h
#define colony_h

#include <string>
#include <memory>
#include <vector>
#include <map>

using namespace std;

#include "building.h"
#include "logistics.h"

/**
 * @brief Klasa zarządzająca budynkami, zawiera w sobie: Logistics f_logisyka
 * 
 */
class Colony{
    private:
        //Wektor zbudowanych budynkow
        vector<unique_ptr<Building>> buildings;

        //Obiekt loogityki - zarzadza wszystkimi parametrami
        //Z tego powodu zeby prosicej bylo sie odwolywac z Game
        Logistics f_logisyka;

    public:
        Colony();
        ~Colony(){};

        map<string,int> UIprntBuildingsSumm() const;
        void UIprntBuilding(string bud) const;

        //BUDOWANIE
        BuildResult UIczyStac(const unique_ptr<Building> &b)const;
        void addBuilding(unique_ptr<Building> b);

        BuildResult UIbuduj(unique_ptr<Building> b);
        DestroyResult UIzburzBudynek(string nazwa);


        //NEXT ROUND
        NextResult UInextRound(); 

        bool sprawdzLvlTerr();

        //SAVE / LOAD
        void saveBuildings(string nazwa_plik);
        void loadBuildings(string nazwa_plik);
        void saveColony(string nazwa_plik);
        void loadColony(string nazwa_plik);

        void save();
        void load();


        //SETTERY
        void setSandbox();
        void setCustom();
        void setRuch(int r);
        void setNazwa();

        //GETTERY
        int getRuch()const;
        int getIloscBudynkow()const;
        int getAllWorkers()const;
        int getDemandWorkers()const;
        int getIlosc(string name)const;
        int getToNextLvlTerr() const;
        int getLvlTerr() const;
  
        string getNazwa() const;
        int getTura() const;
        int getReqEnergy() const;
        int getGenEnergy() const;
        int getReqFood() const;
        int getFood() const;
        int getStone() const;
        int getTitan() const;
};

#endif
