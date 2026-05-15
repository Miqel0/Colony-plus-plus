#include <iostream>
#include <string>
#include <fstream>

using namespace std;
#include "producer.h"

// ==========================================
// KONSTRUKTOR
// ==========================================

Producer:: Producer():Building("XXX",TypBudynku::PRODUCER,0,0,0,0),stoneGen(0),titanGen(0){}
Producer::Producer(string n, double kE,double kK, double kT, double s,int w,double ti):Building(n,TypBudynku::PRODUCER,kE,kK,kT,w),stoneGen(s),titanGen(ti){}

// ==========================================
// OVERRIDE
// ==========================================

double Producer::work(){return stoneGen;}

void Producer::prnt(int il)const{
    prntTablica(name,"Ilosc: ",cleanNum(il),"Koszt energii: ",cleanNum(kosztEnergii),"Pracownicy: ",cleanNum(workers)," "," ","Generowany kamien: ",cleanNum(stoneGen),"Generowany tytan: ",cleanNum(titanGen));
}

void Producer::UIprnt(int il)const{
    if(stoneGen !=0 && titanGen !=0){
        prntTooltipTablica(name,{{"Ilosc: ",cleanNum(il)},{"Koszt energii: ",cleanNum(kosztEnergii)},{"Pracownicy: ",cleanNum(workers)},{"Generowany kamien: ",cleanNum(stoneGen)},{"Generowany tytan: ",cleanNum(titanGen)}});
    }else if( stoneGen!=0 && titanGen==0){
        prntTooltipTablica(name,{{"Ilosc: ",cleanNum(il)},{"Koszt energii: ",cleanNum(kosztEnergii)},{"Pracownicy: ",cleanNum(workers)},{"Generowany kamien: ",cleanNum(stoneGen)}});
    }else if( titanGen!=0 && stoneGen==0){
        prntTooltipTablica(name,{{"Ilosc: ",cleanNum(il)},{"Koszt energii: ",cleanNum(kosztEnergii)},{"Pracownicy: ",cleanNum(workers)},{"Generowany tytan: ",cleanNum(titanGen)}});
    }
}

void Producer::save(ofstream& plik)const{
    Building::save(plik);
    plik<<" "<<stoneGen<<" "<<titanGen<<endl; //pilnowac przy kolejnym doawaniu
}

// ==========================================
// GETTERY
// ==========================================


double Producer::getGenTitan() const{return titanGen;}
