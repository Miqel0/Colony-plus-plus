#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <memory>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;
#include "farm.h"

Farm:: Farm():Building("XXX",TypBudynku::FARM,0,0),foodGen(0),time(0),pType(TypFarm::NIEZNANY){}
Farm::Farm(string n,double k, double f,TypFarm t,int w,int tim):Building(n,TypBudynku::FARM,k,w),foodGen(f),time(tim),pType(t){}

void Farm::prnt()const{
    Building::prnt();
    cout<<" - - - - - - - - - -  - Informacje FARM - - - - - - - - - "<<endl;
    cout<<"Pod type: "<<static_cast<int>(pType)<<endl;
    cout<<"Generowane jedzenie: "<<foodGen<<endl;
    cout<<"Czas potrzebny do urośnięcia: "<<time<<endl;

}


void Farm::save(ofstream& plik)const{
    Building::save(plik);
    plik<<" "<<static_cast<int>(pType)<<" "<<foodGen<<" "<<time<<endl;
}