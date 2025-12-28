#ifndef producer_h
#define producer_h

#include <string>
#include <fstream>


using namespace std;

#include "building.h"

class Producer: public Building{
    double stoneGen;
    double titanGen;
    TypProducer pType;


    public:
    Producer();
    Producer(string n,double k, double s,TypProducer t,int w,double ti);

    double work() override;
    void prnt() const override;
    void save(ofstream& plik) const override;
    int getPType() const override;
    
};

#endif
