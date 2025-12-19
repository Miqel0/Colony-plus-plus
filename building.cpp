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

}

Building:: Building():kosztEnergii(0),type(TypBudynku::NIEZNANY),name("Budynek"),id(next_id++){

}

Building:: Building(string n,TypBudynku t, double k):kosztEnergii(k),type(t),name(n),id(next_id++){
}

string Building:: getName()const {return name;}

void Building:: tick(){

}