#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <memory>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;
#include "energy.h"

Energy:: Energy():Building("XXX",TypBudynku::ENERGY,0,0),enGen(0),pType(TypEnergy::NIEZNANY){}
Energy::Energy(string n,double k, double e,TypEnergy t,int w):Building(n,TypBudynku::ENERGY,k,w),enGen(e),pType(t){}

void Energy::prnt()const{
    Building::prnt();
    cout<<YELLOW<<" - - - - - - - - - -  - Informacje ENERGY - - - - - - - - - "<<RESET<<endl;
    cout<<"Pod type: "<<static_cast<int>(pType)<<endl;
    cout<<"Generowana energia: "<<enGen<<endl;
    


}

void Energy::save(ofstream& plik)const{
    Building::save(plik);
    plik<<" "<<static_cast<int>(pType)<<" "<<enGen<<" "<<endl;
}

double Energy::getEnergy() const{return enGen;}
int Energy::getPType() const{return static_cast<int>(pType);}
