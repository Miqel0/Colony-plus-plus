#include <fstream>
#include <string>
#include <iostream>

using namespace std;
#include "terr.h"

// ==========================================
// KONSTRUKTOR
// ==========================================

Terr:: Terr():Building("XXX",TypBudynku::TERR,0,0,0,0),terrGen(0){}
Terr::Terr(string n, double kE,double kK, double kT, double te,int w):Building(n,TypBudynku::TERR,kE,kK,kT,w),terrGen(te){}

// ==========================================
// OVERRIDE
// ==========================================

double Terr::work(){return terrGen;}

void Terr::prnt(int il)const{
    prntTablica(name,"Ilosc: ",cleanNum(il),"Koszt energii: ",cleanNum(kosztEnergii),"Pracownicy: ",cleanNum(workers),"Wplyw na terraformacje: ",cleanNum(terrGen));
}
void Terr::UIprnt(int il)const{
    prntTooltipTablica(name,{{"Ilosc: ",cleanNum(il)},{"Koszt energii: ",cleanNum(kosztEnergii)},{"Pracownicy: ",cleanNum(workers)},{"Wplywa na terraformacje: ",cleanNum(terrGen)}},"Bardzo fajnie ze zwieksza terraforacje!!!");
}

void Terr::save(ofstream& plik)const{
    Building::save(plik);
    plik<<" "<<terrGen<<" "<<endl;
}

// ==========================================
// GETTERY
// ==========================================

double Terr::getTerr() const{return terrGen;}

