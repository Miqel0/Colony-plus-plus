#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <memory>
#include <string>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

#include "building.h"
#include "energy.h"
#include "colony.h"
#include "enums.h"

int main() {

    Colony kolonia;
  

    kolonia.zbudujBudynek(TypBudynku::ENERGY);
    kolonia.prntBuilding(0);
    kolonia.zbudujBudynek(TypBudynku::NIEZNANY);
    kolonia.prntBuilding(1);
    kolonia.zbudujBudynek(TypBudynku::ENERGY);
    kolonia.zbudujBudynek(TypBudynku::ENERGY);
    kolonia.zbudujBudynek(TypBudynku::ENERGY);
    kolonia.zbudujBudynek(TypBudynku::ENERGY);
    kolonia.zbudujBudynek(TypBudynku::ENERGY);

    kolonia.prntBuildings();
    cout<<kolonia.getIloscBudynkow()<<endl;
    kolonia.zburzBudynek(3);
    cout<<kolonia.getIloscBudynkow()<<endl;
    return 0;
}