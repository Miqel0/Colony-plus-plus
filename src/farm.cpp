#include <iostream>
#include <string>
#include <fstream>

using namespace std;
#include "farm.h"

// ==========================================
// KONSTRUKTOR
// ==========================================

Farm:: Farm():Building("XXX",TypBudynku::FARM,0,0,0,0),foodGen(0),time(0),curr_time(0),pType(TypFarm::NIEZNANY){}
Farm::Farm(string n, double kE,double kK, double kT, double f,TypFarm t,int w,int tim,int ct):Building(n,TypBudynku::FARM,kE,kK,kT,w),foodGen(f),time(tim),curr_time(ct),pType(t){}

// ==========================================
// OVERRIDE
// ==========================================

void Farm::prnt(int il)const{
    //Building::prnt(il);
    prntTablica(name,"Ilosc: ",cleanNum(il),"Koszt energii: ",cleanNum(kosztEnergii),"Pracownicy: ",cleanNum(workers),"Generowane jedzenie: ",cleanNum(foodGen),"Czas potrzebny do urosniecia: ",cleanNum(time)," "," ");
}

void Farm::UIprnt(int il)const{
    prntTooltipTablica(name,{{"Ilosc: ",cleanNum(il)},{"Koszt energii: ",cleanNum(kosztEnergii)},{"Pracownicy: ",cleanNum(workers)},{"Generowane jedzenie: ",cleanNum(foodGen)},{"Czas potrzebny do urosniecia: ",cleanNum(time)}},"Bardzo fajnie ze generuje jedzenie!!!");
}

void Farm::save(ofstream& plik)const{
    Building::save(plik);
    plik<<" "<<static_cast<int>(pType)<<" "<<foodGen<<" "<<time<<" "<<curr_time<<endl; 
}

double Farm::work(){// Przy pracy Farm, jest dodatkowo sprawdzany czas pracy
    curr_time++; //Kazdy Farm produkuje z inna predkoscia (musza urosnac rzeczy)
    if(curr_time==time){//Jak mija konkretny czas - liczba tur - to dopiero wtedy produkuje jedzenie
        curr_time=0;
        return foodGen;
    }else{
        return 0;
    }
}

// ==========================================
// GETTERY
// ==========================================

int Farm::getPType() const{return static_cast<int>(pType);}
