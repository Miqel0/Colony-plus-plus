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

#ifndef logistics_h
#define logistics_h

class Logistics{
    
    int tura;
    double reqEnergy;
    double genEnergy;
    int reqFood;
    int food;


    public:
    Logistics();
    void prnt();
    void nextRound();
    void updateBudynek(Building* budynek);
    void updateZburzBudynek(Building* budynek);

};

#endif
