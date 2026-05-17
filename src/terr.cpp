#include <fstream>
#include <string>
#include <iostream>

using namespace std;
#include "terr.h"

// ==========================================
// KONSTRUKTOR
// ==========================================

Terr:: Terr():Building("XXX",TypBudynku::TERR,0,0,0,0),terrGen(0){}
Terr::Terr(string n, int kE,int kK, int kT, int te,int w):Building(n,TypBudynku::TERR,kE,kK,kT,w),terrGen(te){}

// ==========================================
// OVERRIDE
// ==========================================

int Terr::work(){return terrGen;}

void Terr::prnt(int il)const{
    prntTablica(name,"Ilosc: ",cleanNum(il),"Koszt energii: ",cleanNum(kosztEnergii),"Pracownicy: ",cleanNum(workers),"Wplyw na terraformacje: ",cleanNum(terrGen));
}
void Terr::UIprnt(int il)const{
    prntTooltipTablica(name,{{"Ilosc: ",cleanNum(il)},{"Koszt energii: ",cleanNum(kosztEnergii)},{"Pracownicy: ",cleanNum(workers)},{"Wplywa na terraformacje: ",cleanNum(terrGen)}});
}

void Terr::save(ofstream& plik)const{
    Building::save(plik);
    plik<<" "<<terrGen<<" "<<endl;
}

// ==========================================
// GETTERY
// ==========================================

int Terr::getTerr() const{return terrGen;}

