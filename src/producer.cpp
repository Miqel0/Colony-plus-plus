#include <iostream>
#include <string>
#include <fstream>

using namespace std;
#include "producer.h"

// ==========================================
// KONSTRUKTOR
// ==========================================

Producer:: Producer():Building("XXX",TypBudynku::PRODUCER,0,0,0,0,-1,-1),stoneGen(0),titanGen(0){}
Producer::Producer(string n, int kE,int kK, int kT, int s,int w,int ti, int X_,int Y_):Building(n,TypBudynku::PRODUCER,kE,kK,kT,w,X_,Y_),stoneGen(s),titanGen(ti){}

// ==========================================
// OVERRIDE
// ==========================================

/**
 * @brief WYKONYWANIE PRACY PRZEZ DANY BUDYNEK PODCZAS nextRound, f. virtualna
 *
 * @return int zwracana wartość podczas pracy
 */
int Producer::work(){return stoneGen;}//FIXME WTFFF

/**
 * @brief Funckja wyświetlająca dane danego budynku do tooltipa f.przeciążona
 * 
 * @param il  ilość danego budynku
 */
void Producer::UIprnt(int il)const{
    if(stoneGen !=0 && titanGen !=0){
        prntTooltipTablica(name,{{"Ilosc: ",cleanNum(il)},{"Koszt energii: ",cleanNum(kosztEnergii)},{"Pracownicy: ",cleanNum(workers)},{"Generowany kamien: ",cleanNum(stoneGen)},{"Generowany tytan: ",cleanNum(titanGen)}});
    }else if( stoneGen!=0 && titanGen==0){
        prntTooltipTablica(name,{{"Ilosc: ",cleanNum(il)},{"Koszt energii: ",cleanNum(kosztEnergii)},{"Pracownicy: ",cleanNum(workers)},{"Generowany kamien: ",cleanNum(stoneGen)}});
    }else if( titanGen!=0 && stoneGen==0){
        prntTooltipTablica(name,{{"Ilosc: ",cleanNum(il)},{"Koszt energii: ",cleanNum(kosztEnergii)},{"Pracownicy: ",cleanNum(workers)},{"Generowany tytan: ",cleanNum(titanGen)}});
    }
}

/**
 * @brief Zapisywanie danych budynku do pliku
 * 
 * @param plik plik
 */
void Producer::save(ofstream& plik)const{
    Building::save(plik);
    plik<<" "<<stoneGen<<" "<<titanGen<<endl; //pilnowac przy kolejnym doawaniu
}

// ==========================================
// GETTERY
// ==========================================

int Producer::getGenTitan() const{return titanGen;}
