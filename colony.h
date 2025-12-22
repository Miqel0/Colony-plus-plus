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

using namespace std;
#include "building.h"
#include "enums.h"
#include "energy.h"
#include "farm.h"
#include "housing.h"
#include "logistics.h"

#ifndef colony_h
#define colony_h

class Colony{
    vector<unique_ptr<Building>> buildings;
    int tura;

    int all_workers;
    int demand_workers;

    Logistics f_logisyka;

    public:
    Colony();
    void prnt();
    void addBuilding(unique_ptr<Building> b);

    void nextRound();
    void update();

    void prntBuilding(int nr);
    void zbudujBudynek(TypEnergy typ);
    void zbudujBudynek(TypBudynku typ);
    void zbudujBudynek(TypFarm typ);
    void zbudujBudynek(TypDomy typ);
    void prntBuildings();
    void zburzBudynek(int nr); //dodac warunek ze nie mozna usunac miesdzkalnego, zeby zabrakło mieszkancow
    int getIloscBudynkow();
    void saveBuildings(string nazwa_plik);
    void loadBuildings(string nazwa_plik);
    int getAllWorkers();
    int getDemandWorkers();

    //Dodać prntBuildingsShort() - same nazwy i ID
    
};

#endif