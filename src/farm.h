#ifndef farm_h
#define farm_h

#include <string>

using namespace std;

#include "building.h"

/**
 * @brief Klasa pochodna Building, odpowiada za produkcje jedzenia
 * 
 */
class Farm: public Building{
    private:
        int foodGen;
        int time;
        int curr_time;

    public:
        //KONSTRUKTOR
        Farm();
        Farm(string n, int kE,int kK, int kT, int f,int w,int tim,int ct, int X_,int Y_);
        ~Farm(){};
        
        //OVERRIDE
        int work() override;
        void UIprnt(int il) const override;
        void save(ofstream& plik) const override;

    
};

#endif
