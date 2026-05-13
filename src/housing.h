#ifndef housing_h
#define housing_h

#include <fstream>
#include <string>

using namespace std;

#include "building.h"


class Housing: public Building{


    public:
        //KONSTRUKTOR
        Housing();
        Housing(string n, double kE,double kK, double kT, int r,int w);
        ~Housing(){};

        //OVERRIDE
        void prnt(int il) const override;
        void UIprnt(int il) const override;
        void save(ofstream& plik) const override;
        int getPType() const override;

};

#endif
