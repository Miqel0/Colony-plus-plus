#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;
#include "building.h"

//ID budynku
int Building::next_id=1;

// ==========================================
// KONSTRUKTORY
// ==========================================

Building:: Building():kosztEnergii(0),kosztKamien(0),kosztTytan(0),type(TypBudynku::NIEZNANY),name("Budynek"),id(next_id++),workers(0),residents(0){}
Building:: Building(string n,TypBudynku t, int kE, int kK, int kT,int w):kosztEnergii(kE),kosztKamien(kK),kosztTytan(kT),type(t),name(n),id(next_id++),workers(w), residents(0){}

// ==========================================
// VIRTUALNE
// ==========================================

/**
 * @brief Funkcja virtualna wyświetlająca parametry danego budynku
 * 
 * @param il ilość danego budynku
 */
void Building:: UIprnt(int il)const{}

//
/**
 * @brief WYKONYWANIE PRACY PRZEZ DANY BUDYNEK PODCZAS nextRound, f. virtualna
 * 
 * @return int zwracana wartość podczas pracy
 */
int Building:: work(){return 0;}

/**
 * @brief Zapisywanie danych budynku do pliku
 * 
 * @param plik plik
 */
void Building::save(ofstream& plik)const{
    plik<<static_cast<int>(type)<<" "<<name<<" "<<id<<" "<<kosztEnergii<<" "<<kosztKamien<<" "<<kosztTytan<<" "<<workers;
}

// ==========================================
// LICZNIKI
// ==========================================

void Building:: setId(int i){id=i;}

void Building::updateLicznik(int wczytaneMaxId){
    if(wczytaneMaxId>=next_id){
        next_id=wczytaneMaxId+1;
    }
}

// ==========================================
// GETTERY
// ==========================================

string Building:: getName()const {return name;}
TypBudynku Building::getTyp() const {return type;}
int Building::getResidents() const{return residents;}
int Building::getDemandWorkers()const {return workers;}
int Building::getReqEnergy() const{return kosztEnergii;}
int Building::getKosztKamien() const{return kosztKamien;}
int Building::getKosztTytan() const{return kosztTytan;}
