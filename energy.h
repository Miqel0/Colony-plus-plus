#ifndef energy_h
#define energy_h

#include <string>
#include <fstream>

using namespace std;

#include "building.h"

class Energy: public Building{
    double enGen;
    TypEnergy pType;

    public:
    Energy();
    Energy(string n, double kE,double kK, double kT, double e,TypEnergy t,int w);

    void prnt(int il) const override;
    void save(ofstream& plik) const override;

    int getPType() const override;
    double getEnergy() const;
};

#endif
