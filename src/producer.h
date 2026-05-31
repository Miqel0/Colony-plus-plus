#ifndef producer_h
#define producer_h

#include <string>
#include <fstream>


using namespace std;

#include "building.h"

/**
 * @brief Klasa pochodna Building, odpowiada za produkcje różnych surowców
 * 
 */
class Producer: public Building{
    private:
        int stoneGen;
        int titanGen;

    public:
        //KONSTRUKTORY
        Producer();
        Producer(string n, int kE,int kK, int kT, int s,int w,int ti, int X_,int Y_);
        ~Producer(){};

        //OVERRIDE
        int work() override;
        void UIprnt(int il) const override;
        void save(ofstream& plik) const override;

        //GETTERY
        int getGenTitan() const;


    
};

#endif
