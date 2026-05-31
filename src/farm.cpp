#include <iostream>
#include <string>
#include <fstream>

using namespace std;
#include "farm.h"

// ==========================================
// KONSTRUKTOR
// ==========================================

Farm:: Farm():Building("XXX",TypBudynku::FARM,0,0,0,0,-1,-1),foodGen(0),time(0),curr_time(0){}
Farm::Farm(string n, int kE,int kK, int kT, int f,int w,int tim,int ct, int X_,int Y_):Building(n,TypBudynku::FARM,kE,kK,kT,w,X_,Y_),foodGen(f),time(tim),curr_time(ct){}

// ==========================================
// OVERRIDE
// ==========================================

/**
 * @brief Funckja wyświetlająca dane danego budynku do tooltipa f.przeciążona
 * 
 * @param il  ilość danego budynku
 */
void Farm::UIprnt(int il)const{
    prntTooltipTablica(name,{{"Ilosc: ",cleanNum(il)},{"Koszt energii: ",cleanNum(kosztEnergii)},{"Pracownicy: ",cleanNum(workers)},{"Generowane jedzenie: ",cleanNum(foodGen)},{"Czas potrzebny do urosniecia: ",cleanNum(time)}});
}

/**
 * @brief Zapisywanie danych budynku do pliku
 * 
 * @param plik plik
 */
void Farm::save(ofstream& plik)const{
    Building::save(plik);
    plik<<" "<<foodGen<<" "<<time<<" "<<curr_time<<endl; 
}

/**
 * @brief WYKONYWANIE PRACY PRZEZ DANY BUDYNEK PODCZAS nextRound, f. virtualna
 * Dla farmy jest dodatkowy warunek na czekanie odpowiedniego czasu na wyrośnięcie
 * @return int zwracana wartość podczas pracy
 */
int Farm::work(){// Przy pracy Farm, jest dodatkowo sprawdzany czas pracy
    curr_time++; //Kazdy Farm produkuje z inna predkoscia (musza urosnac rzeczy)
    if(curr_time==time){//Jak mija konkretny czas - liczba tur - to dopiero wtedy produkuje jedzenie
        curr_time=0;
        return foodGen;
    }else{
        return 0;
    }
}
