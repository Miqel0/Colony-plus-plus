#ifndef terr_h
#define terr_h

#include <string>
#include <fstream>

using namespace std;

#include "building.h"


class Terr: public Building{

    private:
        TypTerr pType;
        double terrGen;

    public:

        //KONSTRUKTOR
        Terr();
        Terr(string n, double kE,double kK, double kT, double te,TypTerr t,int w);
        ~Terr(){};

        //OVERRIDE
        void prnt(int il) const override;
        void save(ofstream& plik) const override;
        double work() override;

        //GETTERY
        int getPType() const override;
        double getTerr() const;
};

#endif
