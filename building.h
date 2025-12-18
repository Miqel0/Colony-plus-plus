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
#include "enums.h"

#ifndef building_h
#define building_h

class Building{
    protected:
        TypBudynku type;
        string name;
        double kosztEnergii;
        int id;
        static int next_id;
        
    public:
    virtual void prnt() const;
    Building();
    Building(string n,TypBudynku t, double k);
    virtual void tick();
    string getName() const;
};

#endif