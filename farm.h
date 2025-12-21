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

#ifndef farm_h
#define farm_h

class Farm: public Building{
    double foodGen;
    TypFarm pType;
    int time;

    public:
    Farm();
    Farm(string n,double k, double f,TypFarm t,int w,int tim);
    void prnt() const override;
    void save(ofstream& plik) const override;
    
};

#endif