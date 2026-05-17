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

        //WYSWIETLANIE
        void prnt();
        void prntBuilding(int nr);
        void prntBuilding(string bud);
        void prntBuildings();
        void prntBuildingsShort();
        void prntBuildingsSumm();

        map<string,int> UIprntBuildingsSumm() const;
        void UIprntBuilding(string bud) const;

        //BUDOWANIE
        bool czyBudynek(string bud)const;
        bool czyStac(const unique_ptr<Building> &b)const;
        BuildResult UIczyStac(const unique_ptr<Building> &b)const;
        void addBuilding(unique_ptr<Building> b);

        // bool zbudujEnergy(string n, double kE,double kK, double kT, double e,TypEnergy t,int w);
        // bool zbudujFarm(string n, double kE,double kK, double kT, double f,TypFarm t,int w,int tim,int ct);
        // bool zbudujHousing(string n, double kE,double kK, double kT, int r,TypDomy t,int w);
        // bool zbudujProducer(string n, double kE,double kK, double kT, double s,TypProducer t,int w,double ti);
        // bool zbudujTerr(string n, double kE,double kK, double kT, double te,TypTerr t,int w);

        BuildResult UIbuduj(unique_ptr<Building> b);
        bool buduj(unique_ptr<Building> b);
        void zburzBudynek(string nazwa);
        DestroyResult UIzburzBudynek(string nazwa);


        //NEXT ROUND
        int nextRound();
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
