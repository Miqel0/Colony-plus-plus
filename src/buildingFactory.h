#ifndef buildingfactory_h
#define buildingfactory_h

#include <memory>
#include <string>
#include "building.h"
#include "farm.h"
#include "energy.h"
#include "housing.h"
#include "terr.h"
#include "producer.h"

class BuildingFactory {
    public:
        std::unique_ptr<Building> stworzBudynek(const BuildingInfo& info) {
        std::string typ = info.type;

        
        if (typ == "FARM") {
            
            return std::make_unique<Farm>(info.nazwa, info.reqEnergy, info.kKamien, info.kTytan, info.genInne, info.workers, info.x, 0);
        }
        else if (typ == "ENERGY") {
            return std::make_unique<Energy>(info.nazwa, info.reqEnergy, info.kKamien, info.kTytan, info.genInne, info.workers);
        }
        else if (typ == "HOUSING") {
            return std::make_unique<Housing>(info.nazwa, info.reqEnergy, info.kKamien, info.kTytan, info.genInne, info.workers);
        }
        else if (typ == "PRODUCER") {
            return std::make_unique<Producer>(info.nazwa, info.reqEnergy, info.kKamien, info.kTytan, info.genKamien, info.workers, info.genTytan);
        }
        else if (typ == "TERR") {
            return std::make_unique<Terr>(info.nazwa, info.reqEnergy, info.kKamien, info.kTytan, info.genInne, info.workers);
        }

        return nullptr; 
    }
};
#endif