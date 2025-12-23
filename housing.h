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

#ifndef housing_h
#define housing_h

class Housing: public Building{

    TypDomy pType;

    public:
    Housing();
    Housing(string n,double k, int r,TypDomy t,int w);
    
    void prnt() const override;
    void save(ofstream& plik) const override;


};

#endif
