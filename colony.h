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

    void prnt();

    void prntBuilding(int nr);
    void prntBuildings();
    void prntBuildingsShort();

    bool nextRound();
    void update();


    void addBuilding(unique_ptr<Building> b);

    bool zbudujBudynek(TypEnergy typ);
    bool zbudujBudynek(TypBudynku typ);
    bool zbudujBudynek(TypFarm typ);
    bool zbudujBudynek(TypDomy typ);
    bool zbudujBudynek(TypProducer typ);
    
    void zburzBudynek(int nr); 
    
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

    
    
};

#endif
