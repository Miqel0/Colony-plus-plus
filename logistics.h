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
    

    double reqEnergy;
    double genEnergy;
    double reqFood;
    double food;
    int stone;
    int titan;


    public:
    Logistics();
    void prnt();

    bool nextRound(const vector<unique_ptr<Building>>& budynki);
    
    void updateBudynek(Building* budynek);
    void updateZburzBudynek(Building* budynek);
    
    double getReqEnergy() const;
    double getGenEnergy() const;
    double getReqFood() const;
    double getFood() const;
    int getStone() const;
    int getTitan() const;
    
    void load(double re,double ge,double rf,double f,int s,int ti);
};

#endif
