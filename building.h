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

        int id;
        static int next_id;

        double kosztEnergii;
        
        int workers;
        int residents;
        
    public:

    Building();
    Building(string n,TypBudynku t, double k,int w);

    virtual void prnt() const;
    virtual void save(ofstream& plik) const;
    virtual double work();
    void setId(int i);

    static void updateLicznik(int wczytaneMaxId); 

    string getName() const;
    int getDemandWorkers() const;
    TypBudynku getTyp() const;
    int getResidents() const;
    double getReqEnergy() const;
};

#endif
