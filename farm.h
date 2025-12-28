#ifndef farm_h
#define farm_h

#include <string>

using namespace std;

#include "building.h"

class Farm: public Building{
    double foodGen;
    TypFarm pType;
    int time;
    int curr_time;

    public:
    Farm();
    Farm(string n,double k, double f,TypFarm t,int w,int tim,int ct);

    double work() override;
    void prnt() const override;
    void save(ofstream& plik) const override;
    int getPType() const override;

    
};

#endif
