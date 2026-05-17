#include <iostream>
#include <string>
#include <fstream>

using namespace std;
#include "housing.h"

// ==========================================
// KONSTRUKTOR
// ==========================================

Housing:: Housing():Building("XXX",TypBudynku::HOUSING,0,0,0,0){this->residents=0;}
Housing::Housing(string n, int kE,int kK, int kT, int r,int w):Building(n,TypBudynku::HOUSING,kE,kK,kT,0){this->residents=r;}

// ==========================================
// OVERRIDE
// ==========================================

/**
 * @brief Funckja wyświetlająca dane danego budynku do tooltipa f.przeciążona
 * 
 * @param il  ilość danego budynku
 */
void Housing::UIprnt(int il)const{
    prntTooltipTablica(name,{{"Ilosc: ",cleanNum(il)},{"Koszt energii: ",cleanNum(kosztEnergii)},{"Liczba mieszkancow: ",cleanNum(residents)}});
}

/**
 * @brief Zapisywanie danych budynku do pliku
 * 
 * @param plik plik
 */
void Housing::save(ofstream& plik)const{
    Building::save(plik);
    plik<<" "<<residents<<" "<<endl;
}

