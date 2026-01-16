#include <iostream>
#include <string>
#include <fstream>

using namespace std;
#include "housing.h"

// ==========================================
// KONSTRUKTOR
// ==========================================

Housing:: Housing():Building("XXX",TypBudynku::HOUSING,0,0,0,0),pType(TypDomy::NIEZNANY){this->residents=0;}
Housing::Housing(string n, double kE,double kK, double kT, int r,TypDomy t,int w):Building(n,TypBudynku::HOUSING,kE,kK,kT,0),pType(t){this->residents=r;}

// ==========================================
// OVERRIDE
// ==========================================

void Housing::prnt(int il)const{
    prntTablica(name,"Ilosc: ",cleanNum(il),"Koszt energii: ",cleanNum(kosztEnergii),"Liczba mieszkancow: ", cleanNum(residents)," "," ");
}

void Housing::save(ofstream& plik)const{
    Building::save(plik);
    plik<<" "<<static_cast<int>(pType)<<" "<<residents<<" "<<endl;
}


// ==========================================
// GETTERY
// ==========================================

int Housing::getPType() const{return static_cast<int>(pType);}
