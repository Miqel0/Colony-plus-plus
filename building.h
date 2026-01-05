#ifndef building_h
#define building_h

#include <fstream>
#include <string>

using namespace std;

#include "enums.h"

class Building{
    protected:
        TypBudynku type;
        string name;

        int id;
        static int next_id;

        double kosztEnergii;
        double kosztKamien;
        double kosztTytan;
        int workers;
        int residents;
        
    public:

    Building();
    Building(string n,TypBudynku t, double kE,double kK, double kT,int w);

    virtual void prnt(int il) const;
    virtual void save(ofstream& plik) const;
    virtual double work();
    void setId(int i);

    static void updateLicznik(int wczytaneMaxId); 

    string getName() const;
    int getDemandWorkers() const;
    TypBudynku getTyp() const;

    int getResidents() const;
    double getReqEnergy() const;
    virtual int getPType() const;

    double getKosztKamien() const;
    double getKosztTytan() const;
};

#endif
