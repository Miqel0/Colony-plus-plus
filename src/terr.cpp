#include <fstream>
#include <string>
#include <iostream>

using namespace std;
#include "terr.h"

// ==========================================
// KONSTRUKTOR
// ==========================================

Terr:: Terr():Building("XXX",TypBudynku::TERR,0,0,0,0),terrGen(0),pType(TypTerr::NIEZNANY){}
Terr::Terr(string n, double kE,double kK, double kT, double te,TypTerr t,int w):Building(n,TypBudynku::TERR,kE,kK,kT,w),terrGen(te),pType(t){}

// ==========================================
// OVERRIDE
// ==========================================

double Terr::work(){return terrGen;}

void Terr::prnt(int il)const{
    prntTablica(name,"Ilosc: ",cleanNum(il),"Koszt energii: ",cleanNum(kosztEnergii),"Pracownicy: ",cleanNum(workers),"Wplyw na terraformacje: ",cleanNum(terrGen));
}

void Terr::save(ofstream& plik)const{
    Building::save(plik);
    plik<<" "<<static_cast<int>(pType)<<" "<<terrGen<<" "<<endl;
}

// ==========================================
// GETTERY
// ==========================================

double Terr::getTerr() const{return terrGen;}
int Terr::getPType() const{return static_cast<int>(pType);}
