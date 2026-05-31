#ifndef building_h
#define building_h

#include <fstream>
#include <string>

using namespace std;

#include "utils.h"

/**
 * @brief Główna klasa wszystkich budynków, posiada w sobie podstawowe parametry, oraz funkcje virtualne
 * 
 */
class Building{
    protected:
        TypBudynku type;
        string name;

        int id;
        static int next_id;

        int kosztEnergii;
        int kosztKamien;
        int kosztTytan;
        
        int workers;
        int residents;
        
        int X;
        int Y;
    public:
        //KONSTRUKTORY
        Building();
        Building(string n,TypBudynku t, int kE,int kK, int kT,int w,int X_, int Y_);
        virtual ~Building(){};

        //VIRTUAL
        virtual void save(ofstream& plik) const;
        virtual int work();
        /**
         * @brief Funkcja wyświetlająca dane o budynku
         * 
         * @param il ilość danego budynku
         */
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
        int getReqEnergy() const;
        int getKosztKamien() const;
        int getKosztTytan() const;
        int getID() const;
        int getX() const;
        int getY() const;
        
};

#endif
