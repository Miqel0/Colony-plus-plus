#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <memory>
#include <string>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>

using namespace std;
#include "building.h"
#include "enums.h"
#include "energy.h"
#include "farm.h"
#include "housing.h"
#include "logistics.h"
#include "producer.h"

#ifndef colony_h
#define colony_h

class Colony{
    vector<unique_ptr<Building>> buildings;
    string nazwa_kolonii;
    int tura;

    int all_workers;
    int demand_workers;

    Logistics f_logisyka;

    public:
    Colony();

    void prnt();
    void prntBuilding(int nr);
    void prntBuildings();
    void prntBuildingsShort();

    void nextRound();
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

    void save();

    void setNazwa();
    int getIloscBudynkow();
    int getAllWorkers();
    int getDemandWorkers();
    
    
    
};

#endif
