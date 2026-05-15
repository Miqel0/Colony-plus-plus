#ifndef energy_h
#define energy_h

#include <string>
#include <fstream>

using namespace std;

#include "building.h"

/**
 * @brief Klasa pochodna Building, odpowiada za produkcje prądu
 * 
 */
class Energy: public Building{
    private:
        double enGen;

    public:
        //KONSTRUKTOR
        Energy();
        Energy(string n, double kE,double kK, double kT, double e,int w);
        ~Energy(){};

        //OVERRIDE
        void prnt(int il) const override;
        void UIprnt(int il) const override;
        void save(ofstream& plik) const override;

        //GETTERY
        double getEnergy() const;
        
};

#endif
