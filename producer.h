#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <memory>
#include <string>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;
#include "building.h"

#ifndef producer_h
#define producer_h

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
