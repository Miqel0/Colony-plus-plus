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
        int terrGen;

    public:

        //KONSTRUKTOR
        Terr();
        Terr(string n, int kE,int kK, int kT, int te,int w, int X_,int Y_);
        ~Terr(){};

        //OVERRIDE
        void UIprnt(int il) const override;
        void save(ofstream& plik) const override;
        int work() override;

        //GETTERY
        int getTerr() const;
};

#endif
