#ifndef farm_h
#define farm_h

#include <string>

using namespace std;

#include "building.h"

class Farm: public Building{
    private:
        double foodGen;
        int time;
        int curr_time;

    public:
        //KONSTRUKTOR
        Farm();
        Farm(string n, double kE,double kK, double kT, double f,int w,int tim,int ct);
        ~Farm(){};
        
        //OVERRIDE
        double work() override;
        void prnt(int il) const override;
        void UIprnt(int il) const override;
        void save(ofstream& plik) const override;

        //GETTERY


    
};

#endif
