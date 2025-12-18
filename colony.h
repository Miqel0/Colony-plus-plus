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

#ifndef colony_h
#define colony_h

class Colony{
    vector<unique_ptr<Building>> buildings;
    int tura;
    double reqEnergy;
    double genEnergy;
    double reqFood;
    int workers;

    public:
    Colony();
    void prnt();
    void addBuilding(unique_ptr<Building> b);
    void nextRound();
    void update();
    void prntBuilding(int nr);
    void zbudujBudynek(TypBudynku typ);
    void prntBuildings();
    void zburzBudynek(int nr);
    int getIloscBudynkow();
    
};

#endif