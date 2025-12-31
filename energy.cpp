#include <fstream>
#include <string>
#include <iostream>

using namespace std;
#include "energy.h"

Energy:: Energy():Building("XXX",TypBudynku::ENERGY,0,0),enGen(0),pType(TypEnergy::NIEZNANY){}
Energy::Energy(string n,double k, double e,TypEnergy t,int w):Building(n,TypBudynku::ENERGY,k,w),enGen(e),pType(t){}

void Energy::prnt()const{
    Building::prnt();
    prntTablica1("Informacje ENERGY","Pod type: ",cleanNum(static_cast<int>(pType)),"Generowana energia: ",cleanNum(enGen));
}

void Energy::save(ofstream& plik)const{
    Building::save(plik);
    plik<<" "<<static_cast<int>(pType)<<" "<<enGen<<" "<<endl;
}

double Energy::getEnergy() const{return enGen;}
int Energy::getPType() const{return static_cast<int>(pType);}
