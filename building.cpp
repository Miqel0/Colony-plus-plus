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
    cout<<"<> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <> <>"<<endl;
    cout<<" - - - - - - - - - - - Informacje o budynku - - - - - - - - - "<<endl;
    cout<<"Type: "<<static_cast<int>(type)<<endl;
    cout<<"Nazwa: "<<name<<endl;
    cout<<"ID: "<<id<<endl;
    cout<<"Koszt energii: "<<kosztEnergii<<endl;
    cout<<"Pracownicy: "<<workers<<endl;

}

void Building::save(ofstream& plik)const{
   plik<<static_cast<int>(type)<<" "<<name<<" "<<id<<" "<<kosztEnergii<<" "<<workers;
}

Building:: Building():kosztEnergii(0),type(TypBudynku::NIEZNANY),name("Budynek"),id(next_id++),workers(0){}

Building:: Building(string n,TypBudynku t, double k,int w):kosztEnergii(k),type(t),name(n),id(next_id++),workers(w){}

string Building:: getName()const {return name;}

void Building:: tick(){

}