#ifndef housing_h
#define housing_h

#include <fstream>
#include <string>

using namespace std;

#include "building.h"

/**
 * @brief Klasa pochodna Building, odpowiada za zapewnienie mieszkania dla mieszkańców
 * 
 */
class Housing: public Building{


    public:
        //KONSTRUKTOR
        Housing();
        Housing(string n, int kE,int kK, int kT, int r,int w);
        ~Housing(){};

        //OVERRIDE
        void prnt(int il) const override;
        void UIprnt(int il) const override;
        void save(ofstream& plik) const override;

};

#endif
