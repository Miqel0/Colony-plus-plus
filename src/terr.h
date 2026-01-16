#ifndef terr_h
#define terr_h

#include <string>
#include <fstream>

using namespace std;

#include "building.h"


class Terr: public Building{
    TypTerr pType;
    double terrGen;

    public:
    Terr();
    Terr(string n, double kE,double kK, double kT, double te,TypTerr t,int w);

    void prnt(int il) const override;
    void save(ofstream& plik) const override;

    int getPType() const override;
    double getTerr() const;
    double work();
};

#endif
