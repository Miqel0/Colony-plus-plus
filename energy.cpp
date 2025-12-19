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
#include "energy.h"

Energy:: Energy():Building("XXX",TypBudynku::ENERGY,0),enGen(0),pType(TypEnergy::NIEZNANY){}
Energy::Energy(string n,double k, double e,TypEnergy t):Building(n,TypBudynku::ENERGY,k),enGen(e),pType(t){}

void Energy::prnt()const{
    Building::prnt();
    cout<<"Generowana energia: "<<enGen<<endl;
    


}