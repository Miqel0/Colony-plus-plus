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

#ifndef energy_h
#define energy_h

class Energy: public Building{
    float enGen;
    TypEnergy pType;

    public:
    Energy();
    Energy(string n,double k, double e,TypEnergy t);
    void prnt() const override;
    
};

#endif