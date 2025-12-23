#include <algorithm>
#include <cstdlib>
#include <memory>
#include <ctime>
#include <fstream>
#include <string>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;
#include "building.h"

int Building::next_id=1;

void Building:: prnt()const{
    cout<<YELLOW<<"<> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <>"<<RESET<<endl;
    cout<<YELLOW<<" - - - - - - - - - - - Informacje o budynku - - - - - - - - - "<<RESET<<endl;
    cout<<"Type: "<<static_cast<int>(type)<<endl;
    cout<<"Nazwa: "<<name<<endl;
    cout<<"ID: "<<id<<endl;
    cout<<"Koszt energii: "<<kosztEnergii<<endl;
    cout<<"Pracownicy: "<<workers<<endl;

}

void Building::save(ofstream& plik)const{
   plik<<static_cast<int>(type)<<" "<<name<<" "<<id<<" "<<kosztEnergii<<" "<<workers;
}

Building:: Building():kosztEnergii(0),type(TypBudynku::NIEZNANY),name("Budynek"),id(next_id++),workers(0),residents(0){}

Building:: Building(string n,TypBudynku t, double k,int w):kosztEnergii(k),type(t),name(n),id(next_id++),workers(w){}

string Building:: getName()const {return name;}

void Building:: tick(){

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
