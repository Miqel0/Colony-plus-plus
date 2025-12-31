#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;
#include "building.h"

int Building::next_id=1;

void Building:: prnt()const{
    prntTablica2(name,"Type: ",cleanNum(static_cast<int>(type)),"ID: ",cleanNum(id),"Koszt energii: ",cleanNum(kosztEnergii),"Pracownicy: ",cleanNum(workers));
}

void Building::save(ofstream& plik)const{
   plik<<static_cast<int>(type)<<" "<<name<<" "<<id<<" "<<kosztEnergii<<" "<<workers;
}

Building:: Building():kosztEnergii(0),type(TypBudynku::NIEZNANY),name("Budynek"),id(next_id++),workers(0),residents(0){}

Building:: Building(string n,TypBudynku t, double k,int w):kosztEnergii(k),type(t),name(n),id(next_id++),workers(w), residents(0){}

string Building:: getName()const {return name;}

double Building:: work(){
    return 0;
}

void Building:: setId(int i){id=i;}

void Building::updateLicznik(int wczytaneMaxId){
    if(wczytaneMaxId>=next_id){
    next_id=wczytaneMaxId+1;
    }
}

int Building::getDemandWorkers()const {return workers;}
TypBudynku Building::getTyp() const {return type;}
int Building::getResidents() const{return residents;}
double Building::getReqEnergy() const{return kosztEnergii;}
int Building::getPType() const{return 0;}
