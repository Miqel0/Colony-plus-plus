#include <fstream>
#include <string>
#include <iostream>

using namespace std;
#include "terr.h"

// ==========================================
// KONSTRUKTOR
// ==========================================

Terr:: Terr():Building("XXX",TypBudynku::TERR,0,0,0,0,-1,-1),terrGen(0){}
Terr::Terr(string n, int kE,int kK, int kT, int te,int w, int X_,int Y_):Building(n,TypBudynku::TERR,kE,kK,kT,w,X_,Y_),terrGen(te){}

// ==========================================
// OVERRIDE
// ==========================================

/**
 * @brief WYKONYWANIE PRACY PRZEZ DANY BUDYNEK PODCZAS nextRound, f. virtualna
 *
 * @return int zwracana wartość podczas pracy
 */
int Terr::work(){return terrGen;}

/**
 * @brief Funckja wyświetlająca dane danego budynku do tooltipa f.przeciążona
 * 
 * @param il  ilość danego budynku
 */
void Terr::UIprnt(int il)const{
    prntTooltipTablica(name,{{"prad-",cleanNum(kosztEnergii)},{"kolonisci-",cleanNum(workers)},{"terr+",cleanNum(terrGen)}});
}

/**
 * @brief Zapisywanie danych budynku do pliku
 * 
 * @param plik plik
 */
void Terr::save(ofstream& plik)const{
    Building::save(plik);
    plik<<" "<<terrGen<<" "<<endl;
}

// ==========================================
// GETTERY
// ==========================================

int Terr::getTerr() const{return terrGen;}

