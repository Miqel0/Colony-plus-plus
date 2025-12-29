#ifndef logistics_h
#define logistics_h

#include <vector>
#include <memory>
#include <string>

using namespace std;

#include "building.h"

class Logistics{
    string nazwa_kolonii;
    int tura;
    int ruch;

    int all_workers;
    int demand_workers;
    
    
    double reqEnergy;
    double genEnergy;
    double reqFood;
    double food;
    int stone;
    int titan;


    public:
    Logistics();
    void prnt();
    void prntRound(double f, double s, double t);

    bool czyNextRound(const vector<unique_ptr<Building>>& budynki);
    bool nextRound(const vector<unique_ptr<Building>>& budynki);
    
    void updateBudynek(Building* budynek);
    void updateZburzBudynek(Building* budynek);

    void setNazwa();
    void setTura();
    void setRuch(int r);
    void setAWorkers(int aw);
    void setDWorkers(int dw);


    string getNazwa() const;
    int getTura() const;
    int getRuch() const;
    int getAWorkers() const;
    int getDWorkers() const;
    
    double getReqEnergy() const;
    double getGenEnergy() const;
    double getReqFood() const;
    double getFood() const;
    int getStone() const;
    int getTitan() const;

    void save(string nazwa_plik);
    void load(string nazwa_plik);
    
};

#endif
