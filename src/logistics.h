#ifndef logistics_h
#define logistics_h

#include <vector>
#include <memory>
#include <string>

using namespace std;

#include "building.h"

/**
 * @brief Klasa zarządzająca całą logiką kolonii - wszystkie parametry itp.
 * 
 */
class Logistics{
    private:    
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

        vector<int> progi={10,20,50,100,200,500,1000,2000,5000};//progi lvl

    public:
        //KONSTRUKTOR
        Logistics();
        ~Logistics(){};
        
        //WYSWIETLANIE
        void prnt();
        void prntRound(double f, double s, double t, double te,int n);

        //NEXT ROUND
        int czyNextRound(const vector<unique_ptr<Building>>& budynki);
        NextResult UIczyNextRound(const vector<unique_ptr<Building>>& budynki);
        int nextRound(const vector<unique_ptr<Building>>& budynki);
        NextResult UInextRound(const vector<unique_ptr<Building>>& budynki);
        bool sprawdzLvlTerr();

        //BUDOWANIE
        void updateBudynek(Building* budynek);
        void updateZburzBudynek(Building* budynek);
        pair<float, float>  UIupdateZburzBudynek(Building* budynek);

        //SAVE/LOAD
        void save(string nazwa_plik);
        void load(string nazwa_plik);

        //SETTERY
        void setNazwa();
        void setTura();
        void setRuch(int r);
        void setAWorkers(int aw);
        void setDWorkers(int dw);
        void setStone(double s);
        void setTitan(double t);

        void setSandbox();
        void setCustom();

        //GETTERY
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
            
};

#endif
