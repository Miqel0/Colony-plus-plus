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
    
    double wsp_terr;
    int lvl_terr;
    
    double reqEnergy;
    double genEnergy;
    double reqFood;
    double food;
    int stone;
    int titan;

    vector<int> progi={10,20,50,100,200,500,1000,2000,5000};
    public:
    Logistics();
    ~Logistics(){};
    void prnt();
    void prntRound(double f, double s, double t, double te,int n);

    int czyNextRound(const vector<unique_ptr<Building>>& budynki);
    int nextRound(const vector<unique_ptr<Building>>& budynki);
    
    void updateBudynek(Building* budynek);
    void updateZburzBudynek(Building* budynek);

    bool sprawdzLvlTerr();

    void setNazwa();
    void setTura();
    void setRuch(int r);
    void setAWorkers(int aw);
    void setDWorkers(int dw);
    void setStone(double s);
    void setTitan(double t);

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
    int getLvlTerr() const;
    int getToNextLvlTerr() const;

    void setSandbox();
    void setCustom();

    void save(string nazwa_plik);
    void load(string nazwa_plik);
    
};

#endif
