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
#include "housing.h"
#include "producer.h"

int main() {

    Colony kolonia;
    kolonia.prnt();
    kolonia.zbudujBudynek(TypDomy::REZYDENCJA);
    kolonia.zbudujBudynek(TypEnergy::PANELE);
    //kolonia.prntBuilding(0);
    //kolonia.zbudujBudynek(TypBudynku::NIEZNANY);
    //kolonia.prntBuilding(1);
    //kolonia.zbudujBudynek(TypEnergy::WIATRAK);
    //kolonia.zbudujBudynek(TypEnergy::NIEZNANY);
    //kolonia.zbudujBudynek(TypBudynku::ENERGY);
    kolonia.zbudujBudynek(TypDomy::REZYDENCJA);
    kolonia.zbudujBudynek(TypDomy::REZYDENCJA);
    kolonia.zbudujBudynek(TypDomy::REZYDENCJA);
    //kolonia.zbudujBudynek(TypFarm::POLE);
    //kolonia.zbudujBudynek(TypFarm::NIEZNANY);
    //kolonia.zbudujBudynek(TypFarm::SZKLARNIA);
    //kolonia.zbudujBudynek(TypDomy::NIEZNANY);
    
    //kolonia.zbudujBudynek(TypDomy::BARAK);
    //kolonia.prntBuildings();
    //cout<<kolonia.getIloscBudynkow()<<endl;
    //kolonia.saveBuildings("save.txt");


    //kolonia.loadBuildings("save.txt");
    //kolonia.prntBuildings();
    //kolonia.saveBuildings("save1.txt");
    //kolonia.prntBuildings();
    //cout<<kolonia.getIloscBudynkow()<<endl;
    //kolonia.zbudujBudynek(TypEnergy::WIATRAK);
    //kolonia.prntBuilding(7);

    //kolonia.zbudujBudynek()
    kolonia.zbudujBudynek(TypEnergy::WIATRAK);
    kolonia.zbudujBudynek(TypEnergy::WIATRAK);
    kolonia.zbudujBudynek(TypFarm::POLE);
    kolonia.zbudujBudynek(TypProducer::KOPALNIA_KAMIENIA);
    kolonia.zbudujBudynek(TypProducer::KOPALNIA_TYTANU);
    kolonia.zbudujBudynek(TypProducer::ZAAWANSOWANA_KOPALNIA);

    kolonia.prntBuilding(8);
    kolonia.prntBuilding(9);
    kolonia.prntBuilding(10);
    //kolonia.zbudujBudynek(TypFarm::SZKLARNIA);
    
    kolonia.prntBuildingsShort();
    kolonia.prnt();

    kolonia.nextRound();
    kolonia.prnt();
    kolonia.nextRound();
    kolonia.prnt();
    kolonia.nextRound();
    kolonia.prnt();
    //kolonia.zburzBudynek(1);
    //kolonia.prnt();
    return 0;
}
