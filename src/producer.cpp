#include <iostream>
#include <string>
#include <fstream>

using namespace std;
#include "producer.h"

// ==========================================
// KONSTRUKTOR
// ==========================================

Producer:: Producer():Building("XXX",TypBudynku::PRODUCER,0,0,0,0),stoneGen(0),titanGen(0),pType(TypProducer::NIEZNANY){}
Producer::Producer(string n, double kE,double kK, double kT, double s,TypProducer t,int w,double ti):Building(n,TypBudynku::PRODUCER,kE,kK,kT,w),stoneGen(s),titanGen(ti),pType(t){}

// ==========================================
// OVERRIDE
// ==========================================

double Producer::work(){return stoneGen;}

void Producer::prnt(int il)const{
    prntTablica(name,"Ilosc: ",cleanNum(il),"Koszt energii: ",cleanNum(kosztEnergii),"Pracownicy: ",cleanNum(workers)," "," ","Generowany kamien: ",cleanNum(stoneGen),"Generowany tytan: ",cleanNum(titanGen));
}

void Producer::UIprnt(int il)const{
    if(stoneGen !=0 && titanGen !=0){
        prntTooltipTablica(name,{{"Ilosc: ",cleanNum(il)},{"Koszt energii: ",cleanNum(kosztEnergii)},{"Pracownicy: ",cleanNum(workers)},{"Generowany kamien: ",cleanNum(stoneGen)},{"Generowany tytan: ",cleanNum(titanGen)}},"Bardzo fajnie ze generuje tytan i kamien!!!");
    }else if( stoneGen!=0 && titanGen==0){
        prntTooltipTablica(name,{{"Ilosc: ",cleanNum(il)},{"Koszt energii: ",cleanNum(kosztEnergii)},{"Pracownicy: ",cleanNum(workers)},{"Generowany kamien: ",cleanNum(stoneGen)}},"Bardzo fajnie ze generuje kamien!!!");
    }else if( titanGen!=0 && stoneGen==0){
        prntTooltipTablica(name,{{"Ilosc: ",cleanNum(il)},{"Koszt energii: ",cleanNum(kosztEnergii)},{"Pracownicy: ",cleanNum(workers)},{"Generowany tytan: ",cleanNum(titanGen)}},"Bardzo fajnie ze generuje tytan!!!");
    }
}

void Producer::save(ofstream& plik)const{
    Building::save(plik);
    plik<<" "<<static_cast<int>(pType)<<" "<<stoneGen<<" "<<titanGen<<endl; //pilnowac przy kolejnym doawaniu
}

// ==========================================
// GETTERY
// ==========================================

int Producer::getPType() const{return static_cast<int>(pType);}
TypProducer Producer::getTypeProducer() const{return pType;}
double Producer::getGenTitan() const{return titanGen;}
