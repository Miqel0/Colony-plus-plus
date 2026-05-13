#include <iostream>
#include <string>
#include <fstream>

using namespace std;
#include "housing.h"

// ==========================================
// KONSTRUKTOR
// ==========================================

Housing:: Housing():Building("XXX",TypBudynku::HOUSING,0,0,0,0){this->residents=0;}
Housing::Housing(string n, double kE,double kK, double kT, int r,int w):Building(n,TypBudynku::HOUSING,kE,kK,kT,0){this->residents=r;}

// ==========================================
// OVERRIDE
// ==========================================

void Housing::prnt(int il)const{
    prntTablica(name,"Ilosc: ",cleanNum(il),"Koszt energii: ",cleanNum(kosztEnergii),"Liczba mieszkancow: ", cleanNum(residents)," "," ");
}

void Housing::UIprnt(int il)const{
    prntTooltipTablica(name,{{"Ilosc: ",cleanNum(il)},{"Koszt energii: ",cleanNum(kosztEnergii)},{"Liczba mieszkancow: ",cleanNum(residents)}},"Bardzo fajnie ze zapewnia miejsce dla mieszkancow!!!");
}

void Housing::save(ofstream& plik)const{
    Building::save(plik);
    plik<<" "<<residents<<" "<<endl;
}


// ==========================================
// GETTERY
// ==========================================


