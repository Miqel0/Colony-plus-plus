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
#include "farm.h"

int main() {

    Colony kolonia;
  

    kolonia.zbudujBudynek(TypEnergy::PANELE);
    //kolonia.prntBuilding(0);
    //kolonia.zbudujBudynek(TypBudynku::NIEZNANY);
    //kolonia.prntBuilding(1);
    kolonia.zbudujBudynek(TypEnergy::WIATRAK);
    kolonia.zbudujBudynek(TypEnergy::NIEZNANY);
    kolonia.zbudujBudynek(TypBudynku::ENERGY);
    kolonia.zbudujBudynek(TypFarm::POLE);
    kolonia.zbudujBudynek(TypFarm::NIEZNANY);
    kolonia.zbudujBudynek(TypFarm::SZKLARNIA);

    kolonia.prntBuildings();
    cout<<kolonia.getIloscBudynkow()<<endl;
    kolonia.saveBuildings("save.txt");

    return 0;
}