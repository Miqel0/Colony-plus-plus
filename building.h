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
        static int next_id;//dodac do save
        int workers;
        
    public:
    virtual void prnt() const;
    virtual void save(ofstream& plik) const;
    Building();
    Building(string n,TypBudynku t, double k,int w);
    virtual void tick();
    string getName() const;
};

#endif