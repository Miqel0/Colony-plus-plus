#ifndef producer_h
#define producer_h

#include <string>
#include <fstream>


using namespace std;

#include "building.h"

class Producer: public Building{
    private:
        double stoneGen;
        double titanGen;

    public:
        //KONSTRUKTORY
        Producer();
        Producer(string n, double kE,double kK, double kT, double s,int w,double ti);
        ~Producer(){};

        //OVERRIDE
        double work() override;
        void prnt(int il) const override;
        void UIprnt(int il) const override;
        void save(ofstream& plik) const override;

        //GETTERY
        double getGenTitan() const;


    
};

#endif
