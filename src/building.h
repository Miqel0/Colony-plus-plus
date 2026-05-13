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
        //KONSTRUKTORY
        Building();
        Building(string n,TypBudynku t, double kE,double kK, double kT,int w);
        virtual ~Building(){};

        //VIRTUAL
        virtual void prnt(int il) const;
        virtual void save(ofstream& plik) const;
        virtual double work();
        virtual void UIprnt(int il) const;
        
        //PRZY LOAD
        void setId(int i);

        //LICZENIE ILOSCI ZBUDOWANYCH BUDYNKOW
        static void updateLicznik(int wczytaneMaxId); 

        //GETTERY
        string getName() const;
        int getDemandWorkers() const;
        TypBudynku getTyp() const;
        int getResidents() const;
        double getReqEnergy() const;
        double getKosztKamien() const;
        double getKosztTytan() const;
        
};

#endif
