#include <fstream>
#include <string>
#include <iostream>

using namespace std;
#include "energy.h"

// ==========================================
// KONSTRUKTORY
// ==========================================

Energy:: Energy():Building("XXX",TypBudynku::ENERGY,0,0,0,0),enGen(0){}
Energy::Energy(string n, int kE,int kK, int kT, int e,int w):Building(n,TypBudynku::ENERGY,kE,kK,kT,w),enGen(e){}

// ==========================================
// OVERRIDE
// ==========================================

void Energy::prnt(int il)const{
    prntTablica(name,"Ilosc: ",cleanNum(il),"Koszt energii: ",cleanNum(kosztEnergii),"Pracownicy: ",cleanNum(workers),"Generowana energia: ",cleanNum(enGen));
}

void Energy::UIprnt(int il)const{
    prntTooltipTablica(name,{{"Ilosc: ",cleanNum(il)},{"Koszt energii: ",cleanNum(kosztEnergii)},{"Pracownicy: ",cleanNum(workers)},{"Generowana energia: ",cleanNum(enGen)}});
}

void Energy::save(ofstream& plik)const{
    Building::save(plik);
    plik<<" "<<enGen<<" "<<endl;
}

int Energy::getEnergy() const{return enGen;}

