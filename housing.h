#ifndef housing_h
#define housing_h

#include <fstream>
#include <string>

using namespace std;

#include "building.h"


class Housing: public Building{

    TypDomy pType;

    public:
    Housing();
    Housing(string n, double kE,double kK, double kT, int r,TypDomy t,int w);
    
    void prnt(int il) const override;
    void save(ofstream& plik) const override;
    int getPType() const override;


};

#endif
