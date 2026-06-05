#include <fstream>
#include <string>
#include <iostream>

using namespace std;
#include "energy.h"

// ==========================================
// KONSTRUKTORY
// ==========================================

Energy:: Energy():Building("XXX",TypBudynku::ENERGY,0,0,0,0,-1,-1),enGen(0){}
Energy::Energy(string n, int kE,int kK, int kT, int e,int w, int X_,int Y_):Building(n,TypBudynku::ENERGY,kE,kK,kT,w,X_,Y_),enGen(e){}

// ==========================================
// OVERRIDE
// ==========================================

/**
 * @brief Funckja wyświetlająca dane danego budynku do tooltipa f.przeciążona
 * 
 * @param il  ilość danego budynku
 */
void Energy::UIprnt(int il)const{
    prntTooltipTablica(name,{{"kolonisci-",cleanNum(workers)},{"prad+",cleanNum(enGen)}});
}

/**
 * @brief Zapisywanie danych budynku do pliku
 * 
 * @param plik plik
 */
void Energy::save(ofstream& plik)const{
    Building::save(plik);
    plik<<" "<<enGen<<" "<<endl;
}

// ==========================================
// GETTERY
// ==========================================

int Energy::getEnergy() const{return enGen;}

