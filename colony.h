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
    string nazwa_kolonii;
    int tura;
    int ruch;

    int all_workers;
    int demand_workers;

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

    void zbudujBudynek(TypEnergy typ);
    void zbudujBudynek(TypBudynku typ);
    void zbudujBudynek(TypFarm typ);
    void zbudujBudynek(TypDomy typ);
    void zbudujBudynek(TypProducer typ);
    
    void zburzBudynek(int nr); 
    
    void saveBuildings(string nazwa_plik);
    void loadBuildings(string nazwa_plik);
    void saveColony(string nazwa_plik);
    void loadColony(string nazwa_plik);

    void save();
    void load();

    
    void setRuch(int r);
    void setNazwa();

    int getRuch();
    int getIloscBudynkow();
    int getAllWorkers();
    int getDemandWorkers();

    
    
};

#endif
