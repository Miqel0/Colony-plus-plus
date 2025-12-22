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
#include "housing.h"

Housing:: Housing():Building("XXX",TypBudynku::HOUSING,0,0),pType(TypDomy::NIEZNANY){this->residents=0;}
Housing::Housing(string n,double k, int r,TypDomy t,int w):Building(n,TypBudynku::HOUSING,k,0),pType(t){this->residents=r;}

void Housing::prnt()const{
    Building::prnt();
    cout<<" - - - - - - - - - -  - Informacje HOUSING - - - - - - - - - "<<endl;
    cout<<"Pod type: "<<static_cast<int>(pType)<<endl;
    cout<<"Liczba mieszkancow: "<<residents<<endl;
    


}

void Housing::save(ofstream& plik)const{
    Building::save(plik);
    plik<<" "<<static_cast<int>(pType)<<" "<<residents<<" "<<endl;
}

