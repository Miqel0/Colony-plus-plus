#ifndef colony_h
#define colony_h

#include <string>
#include <memory>
#include <vector>

using namespace std;

#include "building.h"
#include "logistics.h"

class Colony{
    vector<unique_ptr<Building>> buildings;


    Logistics f_logisyka;

    public:
    Colony();
    ~Colony(){};
    void prnt();

    void prntBuilding(int nr);
    void prntBuilding(string bud);
    void prntBuildings();
    void prntBuildingsShort();
    void prntBuildingsSumm();

    int nextRound();
    void update();


    void addBuilding(unique_ptr<Building> b);

    bool zbudujEnergy(string n, double kE,double kK, double kT, double e,TypEnergy t,int w);
    bool zbudujFarm(string n, double kE,double kK, double kT, double f,TypFarm t,int w,int tim,int ct);
    bool zbudujHousing(string n, double kE,double kK, double kT, int r,TypDomy t,int w);
    bool zbudujProducer(string n, double kE,double kK, double kT, double s,TypProducer t,int w,double ti);
    bool zbudujTerr(string n, double kE,double kK, double kT, double te,TypTerr t,int w);

    bool buduj(unique_ptr<Building> b);
    
    void zburzBudynek(int nr); 
    void zburzBudynek(string nazwa);
    
    void saveBuildings(string nazwa_plik);
    void loadBuildings(string nazwa_plik);
    void saveColony(string nazwa_plik);
    void loadColony(string nazwa_plik);

    void save();
    void load();

    
    void setRuch(int r);
    void setNazwa();

    int getRuch()const;
    int getIloscBudynkow()const;
    int getAllWorkers()const;
    int getDemandWorkers()const;
    int getIlosc(string name)const;
    int getToNextLvlTerr() const;

    int getLvlTerr() const;

    bool sprawdzLvlTerr();

    bool czyBudynek(string bud)const;
    bool czyStac(const unique_ptr<Building> &b)const;

    void setSandbox();
    void setCustom();
    
    
};

#endif
