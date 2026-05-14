#ifndef terr_h
#define terr_h

#include <string>
#include <fstream>

using namespace std;

#include "building.h"

/**
 * @brief Klasa pochodna Building, odpowiada za zwiększanie terraformacji
 * 
 */
class Terr: public Building{

    private:
        double terrGen;

    public:

        //KONSTRUKTOR
        Terr();
        Terr(string n, double kE,double kK, double kT, double te,int w);
        ~Terr(){};

        //OVERRIDE
        void prnt(int il) const override;
        void UIprnt(int il) const override;
        void save(ofstream& plik) const override;
        double work() override;

        //GETTERY
        double getTerr() const;
};

#endif
